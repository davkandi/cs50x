from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp


from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    check = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
    stocks = db.execute("SELECT * FROM portfolio WHERE user_id = :user_id", user_id=session["user_id"])
    items = []

    for stock in stocks:
        dicts = lookup(stock["symbol"])
        dicts["shares"] = stock["shares"]
        items.append(dicts)

    return render_template("index.html", stocks=items, cash=usd(check[0]["cash"]))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Must provide Symbol or/and Shares")
        elif isinstance(request.form.get("shares"), float) or not request.form.get("shares").isdigit() or int(request.form.get("shares")) < 1:
            return apology("Invalid shares")

        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("Symbol doesn't exist")

        rows = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])

        if rows[0]["cash"] < stock["price"] * int(request.form.get("shares")):
            return apology("You can't afford it")

        insert = db.execute("INSERT OR IGNORE INTO portfolio (user_id, symbol, shares) VALUES(:user_id, :symbol, :shares)",\
                user_id=session["user_id"], symbol=stock["symbol"], shares=int(request.form.get("shares")))

        if insert == 0:
            db.execute("UPDATE portfolio SET shares = shares + :shares WHERE user_id = :user AND symbol = :symbol", user=session["user_id"], symbol=stock["symbol"], shares=int(request.form.get("shares")))
        #update the cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", user_id=session["user_id"], cash=rows[0]["cash"] - stock["price"] * int(request.form.get("shares")))
        #insert in transaction
        db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES (:user_id, :symbol, :shares, :price)",\
                    user_id=session["user_id"], symbol=stock["symbol"], shares=int(request.form.get("shares")), price=stock["price"])

        return redirect(url_for("index"))
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    if request.method == "GET":
        history = db.execute("SELECT * FROM history WHERE user_id = :user_id", user_id=session["user_id"])
        return render_template("history.html", items=history)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("quote"):
            return apology("Misssing quote")

        quote = lookup(request.form.get("quote"))
        if quote == None:
            return apology("Symbol doesn't exist")

        return render_template("quoted.html", name=quote["name"], symbol=quote["symbol"], price=usd(quote["price"]))
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("Missing username")

        elif not request.form.get("password"):
            return apology("Must provide a password or confirm password")

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords don't match")

        rows = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hashh)", username=request.form.get("username"), hashh=pwd_context.hash(request.form.get("password")))

        if not rows:
            return apology("Username exists")

        session["user_id"] = rows
        return redirect(url_for("index"))
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Must provide Symbol or Shares")
        elif isinstance(request.form.get("shares"), float) or not request.form.get("shares").isdigit() or int(request.form.get("shares")) < 1 :
            return apology("Invalid shares")
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("Symbol not owned")

        rows = db.execute("SELECT * FROM portfolio WHERE user_id = :user AND symbol = :symbol", user=session["user_id"], symbol=stock["symbol"])
        check = db.execute("SELECT cash FROM users WHERE id= :user", user=session["user_id"])

        if len(rows) != 1:
            return apology("Symbol not owned")
        elif rows[0]["shares"] < int(request.form.get("shares")):
            return apology("Too many shares")

        if rows[0]["shares"] == int(request.form.get("shares")):
            db.execute("DELETE FROM portfolio WHERE user_id = :user AND symbol = :symbol", user=session["user_id"], symbol=stock["symbol"])
        else:
            db.execute("UPDATE portfolio SET shares = shares - :sold WHERE user_id = :user AND symbol = :symbol", user=session["user_id"], symbol=stock["symbol"], sold=int(request.form.get("shares")))

        db.execute("UPDATE users SET cash = cash + :cashed WHERE id = :user", user=session["user_id"], cashed=stock["price"] * rows[0]["shares"])

        #insert transaction
        db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES (:user_id, :symbol, :shares, :price)",\
                    user_id=session["user_id"], symbol=stock["symbol"], shares=-int(request.form.get("shares")), price=stock["price"])

        return redirect(url_for("index"))
    else:
        return render_template("sell.html")

@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    """Change password."""
    if request.method == "POST":
        if not request.form.get("password") or not request.form.get("newpassword") or not request.form.get("confirmation"):
            return apology("you need to provide all fields")
        if request.form.get("newpassword") != request.form.get("confirmation"):
            return apology("passwords don't match")
        rows = db.execute("SELECT * FROM users WHERE id=:user", user=session["user_id"])
        print(rows)
        if not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid password")

        db.execute("UPDATE users SET hash=:password", password=pwd_context.hash(request.form.get("newpassword")))
        return redirect(url_for("index"))
    else:
        return render_template("password.html")

@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    if request.method == "POST":
        if not request.form.get("cash") or request.form.get("cash").isdigit() == False:
            return apology("invalid cash")
        rows = db.execute("SELECT * FROM users WHERE id = :userid", userid=session["user_id"])
        db.execute("UPDATE users SET cash=:money", money=rows[0]["cash"] + int(request.form.get("cash")))

        return redirect(url_for("index"))
    else:
        return render_template("cash.html")
