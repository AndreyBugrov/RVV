sudo su
echo -1 > /proc/sys/kernel/perf_event_paranoid
ehco 0 > /proc/sys/kernel/kptr_restrict
exit
cd "/media/andrey/SP PHD U3/00_DESKTOP/Education/Current/Диплом/Программа/RVV"
source scripts/env.sh
python scripts/run.py compilation -d x86 -l info -c release --flame-graph
cd "media/andrey/SP PHD U3/00_DESKTOP/Education/Current/Диплом/Программа/FlameGraph"
perf record -F 99 -a -g -- ../RVV/bin/experiment.out 5 qr_d_sim 1000 1000 ./log.log
perf script --kallsyms=/proc/kallsyms | ./stackcollapse-perf.pl > perf.script
./flamegraph.pl perf.script > flame_graph.svg
/flamegraph.pl --reverse --inverted perf.script > icicle_graph.svg
firefox flame_graph.svg &
firefox icicle_graph.svg &
