#!/bin/bash
echo "" >> seleccion.dat
for((N=100;N<80000;N+=2000))
do
	./seleccion $N >> seleccion.dat
done
