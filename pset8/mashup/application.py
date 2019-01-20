import os
import re
from flask import Flask, jsonify, render_template, request

from cs50 import SQL
from helpers import lookup

# Configure application
app = Flask(__name__)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
def index():
    """Render map"""
    return render_template("index.html")


@app.route("/articles")
def articles():
    """Look up articles for geo"""

    # TODO

    # Check if geo is present
    if not request.args.get("geo"):
        raise RuntimeError("missing geo")

    # Fetch and return news for any particular geo
    return jsonify(lookup(request.args.get("geo")))


@app.route("/search")
def search():
    """Search for places that match query"""

    # TODO

    # Check if q is present
    if not request.args.get("q"):
        raise RuntimeError("missing geo")

    # If there is a comma in query, tokenize by comma
    if "," in request.args.get("q"):
        args = request.args.get("q").split(', ', 2)
    # Else tokenize by whitespaces
    else:
        args = request.args.get("q").split(None, 2)

    # Value of q can be up to 3 place names - city, state, postal code (as the input placeholder says)
    # Case 1 - only 1 name sent - we assume it can be either of the three
    # Case 2 - 2 names sent - first is city, second is state, third (postal) - doesn't matter
    # Case e - all three names are sent - we assume they are sent respectively

    city = ""
    state = ""
    postal = ""

    # Add wildcards
    if len(args) == 1:
        city = "%" + args[0] + "%"
        state = "%" + args[0] + "%"
        postal = "%" + args[0] + "%"
    elif len(args) == 2:
        city = "%" + args[0] + "%"
        state = "%" + args[1] + "%"
        postal = "%"
    else:
        city = "%" + args[0] + "%"
        state = "%" + args[1] + "%"
        postal = "%" + args[2] + "%"

    try:
        # Fetch anything that might be appropriate (by postal code or place name or admin name{1,2,3} or admin code{1,2,3})
        rows = db.execute(("SELECT * FROM places WHERE "
                           "postal_code LIKE :postal OR "
                           "place_name LIKE :city OR "
                           "(admin_name1 LIKE :state OR "
                           "admin_name2 LIKE :state OR "
                           "admin_name3 LIKE :state OR "
                           "admin_code1 LIKE :state OR "
                           "admin_code2 LIKE :state OR "
                           "admin_code3 LIKE :state)"), postal=postal, city=city, state=state)
    except RuntimeError:
        rows = []
    return jsonify(rows)


@app.route("/update")
def update():
    """Find up to 10 places within view"""

    # Ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # Ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # Explode southwest corner into two variables
    sw_lat, sw_lng = map(float, request.args.get("sw").split(","))

    # Explode northeast corner into two variables
    ne_lat, ne_lng = map(float, request.args.get("ne").split(","))

    # Find 10 cities within view, pseudorandomly chosen if more within view
    if sw_lng <= ne_lng:

        # Doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
                          WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
                          GROUP BY country_code, place_name, admin_code1
                          ORDER BY RANDOM()
                          LIMIT 10""",
                          sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # Crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
                          WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
                          GROUP BY country_code, place_name, admin_code1
                          ORDER BY RANDOM()
                          LIMIT 10""",
                          sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # Output places as JSON
    return jsonify(rows)
