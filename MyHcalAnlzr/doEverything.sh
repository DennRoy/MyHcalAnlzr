run=$1
days=$2
lumi=$3
floatday=$4

#run="363427"
#days="39"
#lumi="0.0"
#floatday="08.02"


# cmsRun step
#cd python
#sed -i "s/XXXXXX/${run}/g" localrun_singleFull.py
#cmsRun localrun_singleFull.py
#sed -i "s/${run}/XXXXXX/g" localrun_singleFull.py
#if [[ "$pedrun" != "XXX" ]]; then
#  sed -i "s/XXXXXX/${pedrun}/g" localrun_singlePed.py
#  cmsRun localrun_singlePed.py
#  sed -i "s/${pedrun}/XXXXXX/g" localrun_singlePed.py
#fi
#cd ..


# Digi step: From nano tuple, make one histogram per channel
./macro_nano ${run} ${floatday}


# Process step: From previous histograms, combine kinds (e.g. all HB, all HE, ...)
python3 digi_process.py ${run} ${days} ${lumi} ${floatday}
mv hist_CalibOutput_hadd.root hist_CalibOutput_hadd.root_old
hadd hist_CalibOutput_hadd.root hist_CalibOutputSummary_run*.root


# Make plots: Make trends from values saved in savefile, and get example histograms from rootfile
source makePlots.sh
#mv /eos/user/d/dmroy/www/plots_archive/PED_plots/extrapolations/ExtrapolationFC_H* /eos/user/d/dmroy/www/plots_archive/PED_plots/extrapolations_FC/
