import os
import re

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    try:
        rows = db.execute(("SELECT SUM(shares * type) as shares, company, symbol, type "
                           "FROM transactions "
                           "WHERE user_id = :user_id GROUP BY symbol, type ORDER BY type ASC"), user_id=session["user_id"])
        t_rows = {}
        for row in rows:
            if row["type"] == 1:
                t_rows = {row["symbol"]: row["shares"],
                          "company": row["company"]
                         }

        balance = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    except RuntimeError:
        pass
    return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        if not symbol or not shares or shares < 1:
            flash("Please provide a valid symbol (e.g. GOOGL) and positive number of shares to buy")
            return redirect("/buy")

        result = lookup(symbol)
        if not result:
            flash("Invalid symbol, please try again")
            return redirect("/buy")
        else:
            try:
                row = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
                if not row:
                    flash("User not found, please (re)log in")
                    return redirect("/logout")
                balance = row[0]["cash"]
                if balance >= shares * result["price"]:
                    db.execute("INSERT INTO transactions (user_id, company, symbol, shares, cost, type) VALUES (:user_id, :company, :symbol, :shares, :cost, :type)",
                               user_id=session["user_id"], company=result["name"], symbol=result["symbol"], shares=shares, cost=result["price"], type=1)
                    db.execute("UPDATE users SET cash = cash - :price WHERE id = :id", price=result["price"], id=session["user_id"])
                    flash("Successful operation")
                    return redirect("/")
                else:
                    db.execute("ROLLBACK")
                    flash("No sufficient funds")
                    return redirect("/buy")

            except RuntimeError:
                flash("Some error occurred, please try again")
                return redirect("/buy")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            flash("Please provide a symbol (e.g. GOOGL) to quote for")
            return redirect("/quote")

        result = lookup(symbol)
        if not result:
            flash("Invalid symbol, please try again")
            return redirect("/quote")
        else:
            return render_template("quoted.html", result=result, usd=usd)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmPassword = request.form.get("confirmation")

        if password != confirmPassword:
            flash("Password and password confirmation fields don't match")
            return redirect("/register")

        if not username or not password or not confirmPassword:
            flash("Please provide a username and password")
            return redirect("/register")

        if len(username) < 3 or len(username) > 16:
            flash("Username must be at least 3 characters but not more than 16")
            return redirect("/register")

        if not re.fullmatch(r'(?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,}', password):
            flash("Your password must contain at least one number and one uppercase and lowercase letter, and at least 8 characters")
            return redirect("/register")

        try:
            result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                                username=username, hash=generate_password_hash(password))
            if not result:
                flash("The username is already in use, please try another one")
                return redirect("/register")
            session["user_id"] = result
            return redirect("/")
        except RuntimeError:
            flash("Some error occurred, please try again")
            return redirect("/register")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
