set encoding utf8
set terminal svg size 600,600 enhanced font "Lato,14"
set output "particles.svg"
set xrange[0:100]
set yrange[0:100]
set key off

p "data" w d
