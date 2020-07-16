import pstats
p = pstats.Stats('appprofile.prof')
p.sort_stats('time').print_stats(100)