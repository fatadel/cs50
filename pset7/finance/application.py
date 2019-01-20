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
        # Fetch how many shares of what companies the user has
        rows = db.execute(("SELECT SUM(shares * type) as shares, company, symbol "
                           "FROM transactions "
                           "WHERE user_id = :user_id GROUP BY symbol"), user_id=session["user_id"])
        # Fetch user's current balance
        balance = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

        # Check for errors
        if not balance or len(balance) != 1:
            flash("User not found, please (re)log in")
            return redirect("/logout")

        # Save the current balance
        balance = balance[0]["cash"]
        # Grand total (balance + shares * cost)
        total = balance
        # Table rows are passed to construct the table in the index page
        t_rows = []

        # Iterate over rows
        # Fetch the actual prices for each stock
        # Recalculate the total variable
        # Save the stocks needed to be displayed (if number of shares is >0)
        for row in rows:
            if row["shares"] > 0:
                result = lookup(row["symbol"])
                if not result:
                    flash("Invalid symbol in your transactions list, please try again")
                    return apology("ERROR")
                row["price"] = result["price"]
                total += result["price"] * row["shares"]
                t_rows.append(row)

        return render_template("index.html", rows=t_rows, balance=balance, total=total, usd=usd)

    except RuntimeError:
        flash("Some error occurred, please try again")
        return apology("ERROR")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Process form on POST and show the form on GET requests
    if request.method == "POST":

        # Get symbol and number of shares
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            flash("Please provide a valid symbol (e.g. GOOGL) and positive number of shares to buy")
            return apology("ERROR")

        # Check for errors
        if not symbol or not shares or shares < 1:
            flash("Please provide a valid symbol (e.g. GOOGL) and positive number of shares to buy")
            return apology("ERROR")

        # Look up for actual data
        result = lookup(symbol)
        if not result:
            flash("Invalid symbol, please try again")
            return apology("ERROR")
        else:
            try:
                # Get user's cash
                row = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
                # Check for errors
                if not row or len(row) != 1:
                    flash("User not found, please (re)log in")
                    return redirect("/logout")
                balance = row[0]["cash"]

                # Check if user has enough money and proceed the operations if so
                if balance >= shares * result["price"]:
                    db.execute("INSERT INTO transactions (user_id, company, symbol, shares, cost, type) VALUES (:user_id, :company, :symbol, :shares, :cost, :type)",
                               user_id=session["user_id"], company=result["name"], symbol=result["symbol"], shares=shares, cost=result["price"], type=1)
                    db.execute("UPDATE users SET cash = cash - :price WHERE id = :id",
                               price=result["price"] * shares, id=session["user_id"])
                    flash("Successful operation")
                    return redirect("/")
                else:
                    flash("No sufficient funds")
                    return apology("ERROR")

            except RuntimeError:
                flash("Some error occurred, please try again")
                return apology("ERROR")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Simply fetch all user's transactions
    try:
        rows = db.execute("SELECT * FROM transactions WHERE user_id = :user_id", user_id=session["user_id"])
        return render_template("history.html", rows=rows, usd=usd)
    except RuntimeError:
        flash("Some error occurred, please try again")
        return apology("ERROR")


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

    # Process form on POST, show the form on GET requests
    if request.method == "POST":

        # Get the symbol
        symbol = request.form.get("symbol")

        # Check for errors
        if not symbol:
            flash("Please provide a symbol (e.g. GOOGL) to quote for")
            return apology("ERROR")

        # Look up actual data, checking for errors
        result = lookup(symbol)
        if not result:
            flash("Invalid symbol, please try again")
            return apology("ERROR")
        else:
            return render_template("quoted.html", result=result, usd=usd)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Process form on POST, show the form on GET
    if request.method == "POST":

        # Get user's data
        username = request.form.get("username")
        password = request.form.get("password")
        confirmPassword = request.form.get("confirmation")

        # Check for errors

        # Passwords don't match
        if password != confirmPassword:
            flash("Password and password confirmation fields don't match")
            return apology("ERROR")
        # One or more of the required fields is not present
        if not username or not password or not confirmPassword:
            flash("Please provide a username and password")
            return apology("ERROR")
        # Username too short or too long
        if len(username) < 3 or len(username) > 16:
            flash("Username must be at least 3 characters but not more than 16")
            return apology("ERROR")
        # Password don't contain small Latin letter/capital Latin letter/digit or less than 8 symbols
        if not re.fullmatch(r'(?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,}', password):
            flash("Your password must contain at least one number and one uppercase and lowercase letter, and at least 8 characters")
            return apology("ERROR")

        try:
            # Create new user
            result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                                username=username, hash=generate_password_hash(password))

            # Check if username is not already taken
            if not result:
                flash("The username is already in use, please try another one")
                return apology("ERROR")

            # Log in new user
            session["user_id"] = result
            return redirect("/")

        except RuntimeError:
            flash("Some error occurred, please try again")
            return apology("ERROR")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Process form on POST, show the form on GET requests
    if request.method == "POST":

        # Get symbol and number of shares
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            flash("Please provide a valid symbol (e.g. GOOGL) along with positive number of shares")
            return apology("ERROR")

        # Check for errors
        if not symbol or not shares or shares < 1:
            flash("Please provide a valid symbol (e.g. GOOGL) along with positive number of shares")
            return apology("ERROR")

        try:
            # Fetch user's shares for appropriate symbol
            rows = db.execute(("SELECT SUM(shares * type) as shares, symbol "
                               "FROM transactions "
                               "WHERE user_id = :user_id AND symbol = :symbol GROUP BY symbol"), symbol=symbol, user_id=session["user_id"])

            # Check for errros
            if not rows or len(rows) != 1:
                flash("Quotes not found")
                return apology("ERROR")
            if shares > rows[0]["shares"]:
                flash("Too many shares")
                return apology("ERROR")

            # Fetch actual prices for selling
            result = lookup(rows[0]["symbol"])
            if not result:
                flash("Invalid symbol in your transactions list, please try again")
                return apology("ERROR")

            # Proceed the operation
            db.execute("INSERT INTO transactions (user_id, company, symbol, shares, cost, type) VALUES (:user_id, :company, :symbol, :shares, :cost, :type)",
                       user_id=session["user_id"], company=result["name"], symbol=result["symbol"], shares=shares, cost=result["price"], type=-1)
            db.execute("UPDATE users SET cash = cash + :profit WHERE id = :id",
                       profit=result["price"] * shares, id=session["user_id"])
            flash("Successful operation")
            return redirect("/")

        except RuntimeError:
            flash("Some error occurred, please try again")
            return apology("ERROR")

    else:
        try:
            # Fetch all user's shares for any company
            rows = db.execute(("SELECT SUM(shares * type) as shares, symbol FROM transactions "
                               "WHERE user_id = :user_id GROUP BY symbol"), user_id=session["user_id"])

            # Table rows that will be passed to the template
            t_rows = []

            for row in rows:
                if row["shares"] > 0:
                    # Remember only companies with at least one share
                    t_rows.append(row)
            return render_template("sell.html", rows=t_rows)

        except RuntimeError:
            flash("Some error occurred, please try again")
            return apology("ERROR")


@app.route("/changePassword", methods=["GET", "POST"])
@login_required
def changePassword():
    """Change user's password"""

    # Process form on POST, show the form on GET requests
    if request.method == "POST":

        # Get user's new password and confirmation
        password = request.form.get("password")
        confirmPassword = request.form.get("confirmation")

        # Check for errors

        # Password and confirmation fields don't match
        if password != confirmPassword:
            flash("Password and password confirmation fields don't match")
            return apology("ERROR")
        # One or more fields is not present
        if not password or not confirmPassword:
            flash("Please provide a password")
            return apology("ERROR")
        # Password don't contain small Latin letter/capital Latin letter/digit or less than 8 symbols
        if not re.fullmatch(r'(?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,}', password):
            flash("Your password must contain at least one number and one uppercase and lowercase letter, and at least 8 characters")
            return apology("ERROR")

        try:
            # Update user's password
            db.execute("UPDATE users SET hash = :hash WHERE id = :id", hash=generate_password_hash(password), id=session["user_id"])
            flash("Successful operation")
            return redirect("/")
        except RuntimeError:
            flash("Some error occurred, please try again")
            return apology("ERROR")

    return render_template("changePassword.html")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
