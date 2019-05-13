#!/bin/bash

# drop all env
export -n PYTHONPATH
export -n PYTHONHOME
export -n MANPATH
export -n LIBRARY_PATH
export -n LD_LIBRARY_PATH
export -n TCLLIBPATH
export -n CPATH
export -n C_INCLUDE_PATH
export -n CPLUS_INCLUDE_PATH
export -n PKG_CONFIG_PATH
export -n CMAKE_PREFIX_PATH
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games

env

# module load slurm-singularity
#export PATH=/wang/environment/cluster/slurm-skretch/bin:$PATH
# module load slurm-singularity/testing
export PATH=/wang/environment/cluster/slurm-skretch-testing/bin:$PATH

BASEDIR_DLS_SOFTWARE=/wang/users/s1ext_user1/cluster_home/20190320_demo-dls/software

# load env
. /scif/apps/visionary-dls/scif/env/90-environment.sh

# module load 
export PATH=$BASEDIR_DLS_SOFTWARE/bin:$PATH
export LD_LIBRARY_PATH=$BASEDIR_DLS_SOFTWARE/lib:$LD_LIBRARY_PATH
export SINGULARITYENV_PREPEND_PATH=$BASEDIR_DLS_SOFTWARE/bin:$PATH
export SINGULARITYENV_LD_LIBRARY_PATH=$BASEDIR_DLS_SOFTWARE/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$BASEDIR_DLS_SOFTWARE/lib

# module load ppu-toolchain
export LD_LIBRARY_PATH=/wang/environment/software/jessie/ppu-toolchain/2019-02-21-1/lib:$LD_LIBRARY_PATH
export SINGULARITYENV_LD_LIBRARY_PATH=/wang/environment/software/jessie/ppu-toolchain/2019-02-21-1/lib:$LD_LIBRARY_PATH
export PATH=/wang/environment/software/jessie/ppu-toolchain/2019-02-21-1/bin:/wang/environment/software/jessie/waf/2019-03-15-1/bin:$PATH
export SINGULARITYENV_PREPEND_PATH=/wang/environment/software/jessie/ppu-toolchain/2019-02-21-1/bin:/wang/environment/software/jessie/waf/2019-03-15-1/bin:$PATH
export PYTHONPATH=/wang/environment/software/jessie/ppu-toolchain/2019-02-21-1/lib:$PYTHONPATH

# module load dls2calib
export PYTHONPATH=/wang/data/calibration/hicann-dls:$PYTHONPATH

# exec it
echo `which python`
exec python2 -m ipykernel $@
