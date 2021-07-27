#!/bin/bash
# Author: Wooyoung Jang
# University of Texas at Arlington

# Date: 2021-07-21 (First version)
# Last update: 2021-07-21

# Purpose of the script:
#	This script helps to submit large amount of jobs for DAMSA experiment at 
# RAON to the TORQUE batch system.
# This script is written for use in UTA ATLAS Tier3 cluster system.

# Print out the current time
echo "[submit.sh] Start time: `date -u`"
STARTTIME=`date +%s`
STARTTIME_STRING=`date`

# Survey basic information of the user and working environment
USRNAME=`whoami`
WORKDIR=`pwd`
RUNSCRIPT="$WORKDIR/run.sh"
OUTDIR=$WORKDIR/output
LOGDIR=$OUTDIR/$1/stdout
ERRDIR=$OUTDIR/$1/stderr

# Job specific variables
nruns=100
EXITSTATUS=0

# Test all the runscripts' presence.
for (( i=1; i<=nruns; i++))
do
  if [ ! -e runscripts/run${i}.sh ] ; then
    echo "File 'runscripts/run${i}.sh' does not exists"
    EXITSTATUS=1
  fi
done

if [ $EXITSTATUS -eq 1 ] ; then
  echo "[submit.sh] Exit due to error."
  exit 1
fi

# Loop over 'nruns' jobs
for (( i=1; i<=nruns; i++))
  /usr/bin/qsub \
    -q normal \
    -o log/qsub_run${a}_log.stdout \
    -e err/qsub_run${a}_err.stderr \
    -l walltime=24:00:00,cput=24:00:00 \
    -N "run${a}" \
    runscripts/run${a}.sh
do
done

echo "[submit.sh] End time: `date -u`"
ENDTIME=`date  +%s`
ENDTIME_STRING=`date`

ELAPSED_TIME=`echo "$ENDTIME - $STARTTIME" | bc`
HTIME=`echo "$ELAPSED_TIME/3600" | bc`
MTIME=`echo "($ELAPSED_TIME/60) - ($HTIME*60)" | bc`
STIME=`echo "$ELAPSED_TIME-(($ELAPSED_TIME/60)*60)"|bc`

echo "$JOBCNT jobs are submitted successfully!"
echo "==============================================================================="
echo "  Start time: $STARTTIME_STRING"
echo "    End time: $ENDTIME_STRING"
echo "Elapsed time: ${HTIME}h${MTIME}m${STIME}s"
echo "==============================================================================="
