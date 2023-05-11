import Atlas
from flask import Flask

app = Flask(__name__)


@app.route('/')
def hello_world():
    try:
        tape = Atlas.Tape()
    except Exception as e:
        pass
    
    
    rateValue = Atlas.Dual(0.05)
    tape.registerInput(rateValue)
    dayCounter = Atlas.Thirty360()
    compounding = Atlas.Simple
    frequency = Atlas.Annual
    rate = Atlas.InterestRate(rateValue, dayCounter, compounding, frequency)
    # define zero coupon instrument
    notional = 100
    startDate = Atlas.Date(1, Atlas.August, 2020)
    endDate = Atlas.Date(1, Atlas.August, 2025)
    paymentFrequency = Atlas.Semiannual
    instrument = Atlas.FixedRateBulletInstrument(
        startDate, endDate, paymentFrequency, notional, rate)
    print(tape.printStatus())
    return 'Test'


if __name__ == '__main__':
    app.run()
