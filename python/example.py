import Atlas
import json
import time
from concurrent.futures import ThreadPoolExecutor, as_completed


def buildProd():
    equalPaymentProdData = {
        "STARTDATE": "29082020",
        "ENDDATE": "29082026",
        "NOTIONAL": 100,
        "PAYMENTFREQUENCY": "MONTHLY",
        "RATE": {
            "VALUE": 0.03,
            "COMPOUNDING": "COMPOUNDED",
            "FREQUENCY": "ANNUAL",
            "DAYCOUNTER": "ACT360"
        }
    }

    schema = Atlas.EqualPaymentProductSchema()
    equalPaymentProd = schema.makeObj(equalPaymentProdData)
    return equalPaymentProd


def buildCurve():
    curveData = {
        "NAME": "FLATFORWARDCURVE",
        "REFDATE": "29082020",
        "TYPE": "FLATFORWARD",
        "DAYCOUNTER": "ACT360",
        "ENABLEEXTRAPOLATION": True,
        "RATE": 0.03
    }

    curve = Atlas.makeFlatForwardCurve(curveData)
    return curve


def buildItems():
    equalPaymentProd = buildProd()
    curve = buildCurve()
    return equalPaymentProd, curve


def profilerTest():
    depositData = {
        "STARTDATE": "29082022",
        "ENDDATE": "17082023",
        "NOTIONAL": 100,
        "RATE": {
            "VALUE": 0.03,
            "COMPOUNDING": "SIMPLE",
            "FREQUENCY": "ANNUAL",
            "DAYCOUNTER": "THIRTY360"
        }
    }
    schema = Atlas.DepositSchema()
    deposit = schema.makeObj(depositData)

    equalPaymentProdData = {
        "STARTDATE": "29082022",
        "ENDDATE": "17082023",
        "NOTIONAL": 100,
        "PAYMENTFREQUENCY": "MONTHLY",
        "RATE": {
            "VALUE": 0.03,
            "COMPOUNDING": "SIMPLE",
            "FREQUENCY": "ANNUAL",
            "DAYCOUNTER": "THIRTY360"
        }
    }

    schema = Atlas.EqualPaymentProductSchema()
    equalPaymentProd = schema.makeObj(equalPaymentProdData)

    profiler = Atlas.CashflowProfiler()

    # visit instruments
    profiler.visit(deposit)
    profiler.visit(equalPaymentProd)

    # check results
    interests = profiler.interests()
    redemptions = profiler.redemptions()
    print("Profiler Interests:\t", json.dumps(interests, indent=4))
    print("Profiler redemptions:\t", json.dumps(redemptions, indent=4))


def solverTest():

    # create prodyuct
    equalPaymentProd, curve = buildItems()

    # index cashflows for model / create request
    indexer = Atlas.CashflowIndexer()
    indexer.visit(equalPaymentProd)
    request = Atlas.MarketRequest()
    indexer.setRequest(request)

    model = Atlas.StaticCurveModel(request)
    model.addDiscountCurve("undefined", curve)
    marketData = model.simulate()

    parSolver = Atlas.ParSolver(marketData)

    parSolver.visit(equalPaymentProd)
    print("Rate:\t", parSolver.results())


def npvTest():
    equalPaymentProd, curve = buildItems()

    # index cashflows for model / create request
    indexer = Atlas.CashflowIndexer()
    indexer.visit(equalPaymentProd)

    request = Atlas.MarketRequest()
    indexer.setRequest(request)

    model = Atlas.StaticCurveModel(request)
    model.addDiscountCurve("undefined", curve)
    marketData = model.simulate()

    npvCacl = Atlas.NPVCalculator(marketData)
    npvCacl.visit(equalPaymentProd)
    print("NPV:\t", npvCacl.results())


def multiSolverTime():
    curve = buildCurve()
    prods = []
    for i in range(0, 1000):
        prods.append(buildProd())
        # index cashflows for model / create request
        indexer = Atlas.CashflowIndexer()
        indexer.visit(prods[-1])

    request = Atlas.MarketRequest()
    indexer.setRequest(request)

    model = Atlas.StaticCurveModel(request)
    model.addDiscountCurve("undefined", curve)
    marketData = model.simulate()

    parSolver = Atlas.ParSolver(marketData)
    start = time.time()
    for prod in prods:
        parSolver.visit(prod)
    end = time.time()
    print("MultiSolver Time:\t", end - start)


def secuentialMultiNPVTime():
    curve = buildCurve()
    prods = []

    start = time.time()
    for i in range(0, 1000):
        prods.append(buildProd())
        # index cashflows for model / create request
        indexer = Atlas.CashflowIndexer()
        indexer.visit(prods[-1])
    end = time.time()
    print("Build Time:\t", end - start)

    request = Atlas.MarketRequest()
    start = time.time()
    indexer.setRequest(request)
    end = time.time()
    print("Set Request Time:\t", end - start)

    model = Atlas.StaticCurveModel(request)
    model.addDiscountCurve("undefined", curve)
    marketData = model.simulate()

    npvCacl = Atlas.NPVCalculator(marketData)
    start = time.time()
    for prod in prods:
        npvCacl.visit(prod)
    end = time.time()
    print("MultiNPV Time:\t", end - start)


def parallelMultiNPVTime():
    curve = buildCurve()
    executor = ThreadPoolExecutor()
    futures = []
    start = time.time()
    for i in range(0, 1000):
        futures.append(executor.submit(buildProd))

    prods = []
    for future in as_completed(futures):
        prods.append(future.result())
    end = time.time()
    print("MultiNPV Time:\t", end - start)


if __name__ == '__main__':
    profilerTest()
    # solverTest()
    # npvTest()
    # multiSolverTime()
    # secuentialMultiNPVTime()
    # parallelMultiNPVTime()
