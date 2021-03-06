import benchmarks
from matplotlib.pylab import *
from numpy import *

testcase = 'cloud_wall_scaling_jugene.xml'

all_charges, all_tolerances, all_cores, timing = benchmarks.read(testcase)

def plot_timing(testcase, charges, tolerance):
    ixcha = all_charges.index(charges)
    ixtol = all_tolerances.index(tolerance)

    methods = timing.keys()
    methods.sort()

    for method in methods:
      data = timing[method]
      if not all(isnan(data[ixcha,ixtol,:])):
        loglog(all_cores, data[ixcha,ixtol,:], 
                   label=method, **benchmarks.fmt(method))

        #    legend()
    xlabel('#cores')
    ylabel('Time [s]')

def plot_efficiency(testcase, charges, tolerance):
    ixcha = all_charges.index(charges)
    ixtol = all_tolerances.index(tolerance)

    methods = timing.keys()
    methods.sort()

    minimal_time = None
    # look for the minimal timing
    for method in methods:
        methodtimes = timing[method][ixcha,ixtol,:]
        if all(isnan(methodtimes)): continue
        i = 0
        while (isnan(methodtimes[i])): i += 1
        serial_time = all_cores[i] * methodtimes[i]
        if minimal_time is None or serial_time < minimal_time:
            minimal_time = serial_time

    if minimal_time is None:
        print "No data for charges={} tolerance={}".format(charges, tolerance)
        return
            
    for method in methods:
      data = timing[method]
      if not all(isnan(data[ixcha,ixtol,:])):
        semilogx(all_cores, minimal_time/(data[ixcha,ixtol,:]*all_cores), 
                     label=method, **benchmarks.fmt(method))

    xlim((1, charges/200.))
    xlabel('#cores')
    ylabel('Efficiency')

figure()
# subplot(231, title='8100 charges, tol 1e-3')
# plot_timing(testcase, 8100, 1e-3)

# subplot(232, title='8100 charges, tol 1e-3')
# plot_efficiency(testcase, 8100, 1e-3)
# legend()

# subplot(233, title='102900 charges, tol 1e-3')
# plot_efficiency(testcase, 102900, 1e-3)

subplot(221, title='1012500 charges, tol 1e-3')
plot_efficiency(testcase, 1012500, 1e-3)

subplot(222, title='9830400 charges, tol 1e-3')
plot_efficiency(testcase, 9830400, 1e-3)

subplot(223, title='1012500000 charges, tol 1e-3')
plot_efficiency(testcase, 1012500000, 1e-3)

# create legend
methods = []
artists = []
for method in benchmarks.method2format:
    methods.append(method)
    artists.append(
        Line2D((0,0),(0,1),
               **benchmarks.method2format[method]))
    
subplot(221)
legend(artists, methods, bbox_to_anchor=(0.0, 1.1))

show()
