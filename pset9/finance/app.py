import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import date

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    if request.method == "GET":
        share_query = db.execute("SELECT symbol, SUM(shares) as total_shares FROM purchases WHERE user_id = ? GROUP BY symbol", session['user_id'])
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session['user_id'])[0]['cash']
        total = cash

        lookup_values = {}
        for share in share_query:
            lookup_values[share["symbol"]] = lookup(share["symbol"])["price"]
            total += lookup_values[share["symbol"]] * share["total_shares"]

    print(share_query)
    return render_template("index.html", cash=cash, shares=share_query, grand_total=total, lookup_values=lookup_values)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy share of stocks"""
    if request.method == "POST":

        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if quote is None:
            return apology("Symbol does not exist")

        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Please enter a valid number")

        if shares < 1:
            return apology("Please enter an integer above 0 for amount of shares.")

        price = shares * quote["price"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session['user_id'])[0]['cash']
        id = db.execute("SELECT id FROM users WHERE id = ?", session['user_id'])[0]['id']
        current_date = date.today()
        type = "bought"

        new_cash = cash - price
        if new_cash < 0:
            return apology("Not enough money")

        # Store purchases in finance.db
        db.execute("INSERT INTO purchases (user_id, symbol, shares, price, date, type) VALUES (?, ?, ?, ?, ?, ?)", id, symbol, shares, price, current_date, type)

        # update the amount of cash after purchase
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session['user_id'])

        flash("purchase complete")

        return redirect("/")

    if request.method == "GET":
        return render_template("/buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    if request.method == "GET":
        history = db.execute("SELECT * FROM purchases WHERE user_id = ?", session['user_id'])
        print(history)
    return render_template("history.html", data=history)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":

        symbol = request.form.get("symbol")
        quote = lookup(symbol)

        if quote is None:
            return apology("Symbol does not exist")

        return render_template("quoted.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":
        password_value = request.form.get("password")
        print(password_value)
        confirmation = request.form.get("confirmation")

        if password_value != confirmation:
            return apology("Passwords do not match")

        if password_value == "" or confirmation == "":
            return apology("Password and confirmation cannot be left blank")

        name = request.form.get("username")
        if name == "":
            return apology("You must enter a username")

        if db.execute("SELECT username FROM users WHERE username = ?", name):
            return apology("Username already exists")

        password = generate_password_hash(password_value, method='pbkdf2:sha256', salt_length=8)

    # Register user
    db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", name, password)

    return redirect("/login")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        share_query = db.execute("SELECT symbol, SUM(shares) as shares FROM purchases WHERE user_id = ? GROUP BY symbol", session['user_id'])
        return render_template("sell.html", shares=share_query)

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        quote = lookup(symbol)["price"]
        price = shares * quote
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session['user_id'])[0]['cash']
        id = db.execute("SELECT id FROM users WHERE id = ?", session['user_id'])
        current_date = date.today()
        type = "sold"

        new_cash = cash + price

        share_query = db.execute("SELECT SUM(shares) as shares FROM purchases WHERE symbol = ? AND user_id = ?", symbol, session['user_id'])[0]['shares']

        if int(shares) > share_query:
            return apology("Not enough shares")

        shares *= - 1

        # Store purchase in database
        db.execute("INSERT INTO purchases (user_id, symbol, shares, price, date, type) VALUES (?, ?, ?, ?, ?, ?)", id[0]['id'], symbol, shares, price, current_date, type)

        # update the amount of cash after purchase
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session['user_id'])

        flash("sold")
        return redirect("/")

@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    if request.method == "POST":
        cash = request.form.get("cash")
        new_cash = int(cash) + db.execute("SELECT cash FROM users WHERE id = ?", session['user_id'])[0]['cash']

        # update the amount of cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session['user_id'])

        flash("cash updated")

    return render_template("/addcash.html")