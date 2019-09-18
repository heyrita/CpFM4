#!/bin/bash
echo "Bash version ${BASH_VERSION}..."
LANG=en_US
LC_NUMERIC=en_US.UTF-8
##########################################################################################
source /home/rita/software/root/bin/thisroot.sh
source /home/rita/Tools/Geant4/share/Geant4-10.5.1/geant4make/geant4make.sh
alias g4cmake="cmake -DGeant4_DIR=/home/rita/Tools/Geant4/lib/Geant4-10.5.1/"

#g4conf495
#ROOT
#source /home/gred/root_v6.08.06/root_v6.08.06-install/bin/thisroot.sh
#source /home/gred/root_34_36/bin/thisroot.sh;
#GEANT4.10 --> defined in .bashrc
#alias heprapp="java -jar /home/gred/geant4.10.02.p02/workdir/HepRApp.jar"
#
#alias g4conf="source /home/gred/geant4.10.02.p02/geant4.10.02.p02-build/geant4
##########################################################################################
