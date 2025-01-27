#!/usr/bin/env python
# coding: utf-8

# In[1]:


import ROOT
import os, sys
from array import array


def GetFullGraph(subdet, alpha, beta, size, what): # what in ["daysince", "lumi", "floatday"]
  #subd = 0 if subdet=="HB" else 1
  if subdet=="HB": subd = 0
  elif subdet=="HE": subd = 1
  elif subdet=="HF": subd = 2
  elif subdet=="HO": subd = 3
  if what=="daysince": xval = 2
  elif what=="lumi": xval = 1
  elif what=="floatday": xval = 3
  vals = {}
  with open('SaveFile.txt') as savefile:
    lines = [line.rstrip() for line in savefile]
  if lines != sorted(lines): # Sort file
    print("Going to sort")
    os.system('mv SaveFile.txt SaveFile.txt_old')
    # First remove duplicate entries with different result: Save newer one
    lines2 = []
    contains = []
    for l in lines:
      category = l.split()[:-1]
      if category not in contains:
        contains.append(category)
      else:
        delme=""
        for k in lines2:
          if k.split()[:-1]==category and k!=l:
            delme=k
            break
        if delme!="":
          lines2.remove(delme)
          #print('Going to replace "',delme,'" with "',l,'"')
      lines2.append(l)
    lastline = ""
    with open('SaveFile.txt', 'w') as savefile:
      for l in sorted(lines2):
        if l==lastline: continue # Remove exact duplicates
        if l.split()[-1] == "0": continue # Ignore empty entries
        savefile.write(l+"\n")
        lastline = l
    
  # Content per line:
  # Run, Lumi, Day since 5th July, Day as float, 0/1 for HB/HE, 0/1 for Small/Large, Mean/RMS, Value
  for l in lines:
    stuff = l.split(" ")
    if stuff[4]==str(subd) and stuff[5]==str(size) and stuff[6]==alpha+beta:
      vals[float(stuff[xval])] = float(stuff[7])

  x = []
  y = []
  for key, value in sorted(vals.items()):
    x.append(key)
    y.append(value)
  if len(x)==0: return None
  gr = ROOT.TGraph(len(x), array('d', x), array('d', y))
  return gr

def GetDay(run):
  with open('SaveFile.txt') as savefile:
    lines = [line.rstrip() for line in savefile]
  for l in lines:
    if l.startswith(run):
      return l.split()[3]

def MinMaxAxis(minv, maxv, mindist):
  thismin = min(minv)
  thismax = max(maxv)
  if thismax-thismin < mindist:
    diff = mindist - (thismax-thismin)
    thismax += diff/2
    thismin -= diff/2
  if thismin - (thismax-thismin)*0.2 < 0:
    thismin = thismax*0.2/1.2
  return thismin, thismax



##### Start
ROOT.gStyle.SetOptFit(1)
ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetBatch(True)
if len(sys.argv)>1:
  inputfile = sys.argv[1]
else:
  inputfile = "hist_Localoutput5_digi.root"
output = inputfile.split(".")[0]+"/"
if not os.path.isdir(output): os.mkdir(output)
fin=ROOT.TFile.Open(inputfile, "READ")
#fin.ls()

if len(sys.argv)>2 and sys.argv[2] in ["daysince", "lumi", "floatday"]:
  dowhat = sys.argv[2]
else:
  dowhat = "daysince"

if dowhat == "daysince":
  xtitle = "Days since 5th July"
elif dowhat == "lumi":
  xtitle = "Luminosity [fb^{-1}]"
elif dowhat == "floatday":
  xtitle = "Day"

runstoplot = []
if len(sys.argv)>3:
  runstoplot = sys.argv[3:]


##### Prepare trend plots
gr = {}
limits = {}
for subdet in ["HB", "HE", "HF", "HO"]:
  gr[subdet] = {}
  limits[subdet+"MeanRMS"] = [999, 0]
  limits[subdet+"RMSMean"] = [999, 0]
  limits[subdet+"MeanMean"] = [999, 0]
  for alpha in ["Mean", "RMS"]:
    gr[subdet][alpha] = {}
    for beta in ["Mean", "RMS"]:
      if alpha=="RMS" and beta=="RMS": continue
      gr[subdet][alpha][beta] = {}
      for size in [1, 0, 2, 3, 4]: # Large and Small, and HF/HO, and Special
        #gr[subdet][alpha][beta][size] = fin.Get(subdet+alpha+"_of_ped"+beta+"_sipmT"+str(size))
        gr[subdet][alpha][beta][size] = GetFullGraph(subdet, alpha, beta, size, dowhat)
        if gr[subdet][alpha][beta][size] is None: continue
        thismin = ROOT.TMath.MinElement(gr[subdet][alpha][beta][size].GetN(), gr[subdet][alpha][beta][size].GetY())
        thismax = ROOT.TMath.MaxElement(gr[subdet][alpha][beta][size].GetN(), gr[subdet][alpha][beta][size].GetY())
        if limits[subdet+alpha+beta][0] > thismin: limits[subdet+alpha+beta][0] = thismin
        if limits[subdet+alpha+beta][1] < thismax: limits[subdet+alpha+beta][1] = thismax

grdict = {}
grdict["Mean of per-channel pedestal RMS in HBHE"] = ["Mean", "RMS", "HB", "HE"]
grdict["RMS of per-channel pedestal Mean in HBHE"] = ["RMS", "Mean", "HB", "HE"]
grdict["Mean of per-channel pedestal Mean in HBHE"] = ["Mean", "Mean", "HB", "HE"]
grdict["Mean of per-channel pedestal RMS in HF"] = ["Mean", "RMS", "HF"]
grdict["RMS of per-channel pedestal Mean in HF"] = ["RMS", "Mean", "HF"]
grdict["Mean of per-channel pedestal Mean in HF"] = ["Mean", "Mean", "HF"]
grdict["Mean of per-channel pedestal RMS in HO"] = ["Mean", "RMS", "HO"]
grdict["RMS of per-channel pedestal Mean in HO"] = ["RMS", "Mean", "HO"]
grdict["Mean of per-channel pedestal Mean in HO"] = ["Mean", "Mean", "HO"]
c = []
legend = []

##### Draw PED trends
for title in grdict:
  c.append(ROOT.TCanvas( 'c'+str(len(c)+1), 'c'+str(len(c)+1), 800, 800 ))
  c[-1].SetLeftMargin(0.12)
  c[-1].cd()
  legend.append(ROOT.TLegend(0.12,0.8,0.9,0.9))
  legend[-1].SetNColumns(2)
  j=0
  alpha = grdict[title][0]
  beta = grdict[title][1]
  subdets = grdict[title][2:]
  #thismin = min(limits[subd+alpha+beta][0] for subd in subdets)
  #thismax = max(limits[subd+alpha+beta][1] for subd in subdets)
  thismin, thismax = MinMaxAxis((limits[subd+alpha+beta][0] for subd in subdets), (limits[subd+alpha+beta][1] for subd in subdets), 0.5)
  for subdet in subdets:
    if subdet=="HB": color = ROOT.kBlue
    elif subdet=="HE": color = ROOT.kGreen
    elif subdet=="HF": color = ROOT.kRed
    elif subdet=="HO": color = ROOT.kBlack
    for size in [1, 0, 2, 3, 4]:
      if gr[subdet][alpha][beta][size] is None: continue
      if size in [0,2]:
        tcolor = color
        marker = 21
        line = 1
      elif size==1:
        tcolor = color+3
        marker = 20
        line = 4
      elif size==3:
        tcolor = color-7
        marker = 22
        line = 9
      elif size==4:
        tcolor = color-5
        marker = 23
        line = 10
      gr[subdet][alpha][beta][size].SetLineColor(tcolor)
      gr[subdet][alpha][beta][size].SetLineStyle(line)
      gr[subdet][alpha][beta][size].SetMarkerStyle(marker)
      gr[subdet][alpha][beta][size].SetMarkerColor(tcolor)
      if j==0:
        gr[subdet][alpha][beta][size].SetTitle(title)
        gr[subdet][alpha][beta][size].GetXaxis().SetTitle(xtitle)
        gr[subdet][alpha][beta][size].GetXaxis().SetDecimals()
        #gr[subdet][alpha][beta][size].GetXaxis().SetMaxDigits(3)
        if subdet=="HF": ytitle = "ADC (QIE10)"
        elif subdet=="HO": ytitle = "ADC (QIE8)"
        else: ytitle = "ADC (QIE11)"
        gr[subdet][alpha][beta][size].GetYaxis().SetTitle(ytitle)
        gr[subdet][alpha][beta][size].SetMinimum(thismin - (thismax-thismin)*0.2)
        gr[subdet][alpha][beta][size].SetMaximum(thismax + (thismax-thismin)*0.3)
        gr[subdet][alpha][beta][size].Draw()
      else:
        gr[subdet][alpha][beta][size].Draw("PL same")
      sizename = "SiPM" if subdet=="HO" else "PM"
      if subdet in ["HB", "HE"]:
        if size==0: sizename = "Small SiPM"
        elif size==1: sizename = "Large SiPM"
        elif size==3: sizename = "iphi in [35,36,37,38]" 
        elif size==4: sizename = "iphi in [1,2,71,72]" 
      label = subdet + " " + sizename + ""
      legend[-1].AddEntry(gr[subdet][alpha][beta][size], label, "pl")
      j+=1
  legend[-1].Draw()
  c[-1].Draw()
  c[-1].SaveAs(output+title.replace(" ", "_")+".png")
  c[-1].SaveAs(output+title.replace(" ", "_")+".pdf")


##### Prepare runs for histograms
if runstoplot!=[]:
  runs = runstoplot
else:
  allruns = []
  for name in fin.GetListOfKeys():
    name = name.GetName()
    if "run" in name:
      run = name.split("run")[1]
      if run not in allruns:
        allruns.append(run)
  allruns.sort()
  ## Select only 4 runs: First, middle, last
  if False:
    runs = [allruns[0]]
    if len(allruns)>3:
      runs.append(allruns[int(len(allruns)/3)])
      runs.append(allruns[int(len(allruns)/3*2)])
    elif len(allruns)>2:
      runs.append(allruns[int(len(allruns)/2)])
    if len(allruns)>1: runs.append(allruns[-1])
  ## Or do all runs
  if False:
    runs = allruns
  ## Or do most recent ones:
  if True:
    step = 1
    runs = [allruns[-1]]
    runs.append(allruns[-1-step])
    runs.append(allruns[-1-2*step])
    runs.append(allruns[-1-3*step])


##### Collect histogram data
h = {}
hdepth = {}
hphi = {}
maxlimit = {}
for subdet in ["HB", "HE"]:
  h[subdet] = {}
  hdepth[subdet] = {}
  hphi[subdet] = {}

  for size in [1, 0]: # Large and Small
    h[subdet][size] = {}
    for alpha in ["Mean", "RMS"]:
      if alpha+subdet not in maxlimit: maxlimit[alpha+subdet] = [0, 999, 0] # Highest y-axis value, Smallest bin with entry, Highest bin with entry
      h[subdet][size][alpha] = {}
      for run in runs:
        h[subdet][size][alpha][run] = fin.Get(subdet+"_sipmT"+str(size)+"ped"+alpha+"_run"+run)
        maxval = h[subdet][size][alpha][run].GetMaximum()
        leftbin = h[subdet][size][alpha][run].GetBinCenter(h[subdet][size][alpha][run].FindFirstBinAbove(0))
        rightbin = h[subdet][size][alpha][run].GetBinCenter(h[subdet][size][alpha][run].FindLastBinAbove(0))
        if maxlimit[alpha+subdet][0] < maxval: maxlimit[alpha+subdet][0] = maxval
        if maxlimit[alpha+subdet][1] > leftbin: maxlimit[alpha+subdet][1] = leftbin
        if maxlimit[alpha+subdet][2] < rightbin: maxlimit[alpha+subdet][2] = rightbin
  for size in [1, 0]: 
    for alpha in ["Mean", "RMS"]:
      h[subdet][size][alpha][runs[-1]].GetXaxis().SetRangeUser(maxlimit[alpha+subdet][1], maxlimit[alpha+subdet][2])
      h[subdet][size][alpha][runs[-1]].GetYaxis().SetRangeUser(0, maxlimit[alpha+subdet][0]*1.3)

  depths = [1, 2, 3, 4, 5, 6, 7] if subdet=="HE" else [1, 2, 3, 4]
  for depth in depths:
    hdepth[subdet][depth] = {}
    for alpha in ["Mean", "RMS"]:
      if "depth"+alpha+subdet not in maxlimit: maxlimit["depth"+alpha+subdet] = [0, 999, 0]
      hdepth[subdet][depth][alpha] = {}
      for run in runs:
        hdepth[subdet][depth][alpha][run] = fin.Get(subdet+"_depth"+str(depth-1)+"ped"+alpha+"_run"+run)
        maxval = hdepth[subdet][depth][alpha][run].GetMaximum()
        leftbin = hdepth[subdet][depth][alpha][run].GetBinCenter(hdepth[subdet][depth][alpha][run].FindFirstBinAbove(0))
        rightbin = hdepth[subdet][depth][alpha][run].GetBinCenter(hdepth[subdet][depth][alpha][run].FindLastBinAbove(0))
        if maxlimit["depth"+alpha+subdet][0] < maxval: maxlimit["depth"+alpha+subdet][0] = maxval
        if maxlimit["depth"+alpha+subdet][1] > leftbin: maxlimit["depth"+alpha+subdet][1] = leftbin
        if maxlimit["depth"+alpha+subdet][2] < rightbin: maxlimit["depth"+alpha+subdet][2] = rightbin
  for depth in depths:
    for alpha in ["Mean", "RMS"]:
      hdepth[subdet][depth][alpha][runs[-1]].GetXaxis().SetRangeUser(maxlimit["depth"+alpha+subdet][1], maxlimit["depth"+alpha+subdet][2])
      hdepth[subdet][depth][alpha][runs[-1]].GetYaxis().SetRangeUser(0, maxlimit["depth"+alpha+subdet][0]*1.3)

  for size in [1, 0]: # Large and Small
    hphi[subdet][size] = {}
    for phi in [0, 1]: # horizonal and vertical
      hphi[subdet][size][phi] = {}
      for alpha in ["Mean", "RMS"]:
        if "phi"+alpha+subdet not in maxlimit: maxlimit["phi"+alpha+subdet] = [0, 999, 0]
        hphi[subdet][size][phi][alpha] = {}
        for run in runs:
          hphi[subdet][size][phi][alpha][run] = fin.Get(subdet+"_sipmT"+str(size)+"_phi"+str(phi)+"ped"+alpha+"_run"+run)
          maxval = hphi[subdet][size][phi][alpha][run].GetMaximum()
          leftbin = hphi[subdet][size][phi][alpha][run].GetBinCenter(hphi[subdet][size][phi][alpha][run].FindFirstBinAbove(0))
          rightbin = hphi[subdet][size][phi][alpha][run].GetBinCenter(hphi[subdet][size][phi][alpha][run].FindLastBinAbove(0))
          if maxlimit["phi"+alpha+subdet][0] < maxval: maxlimit["phi"+alpha+subdet][0] = maxval
          if maxlimit["phi"+alpha+subdet][1] > leftbin: maxlimit["phi"+alpha+subdet][1] = leftbin
          if maxlimit["phi"+alpha+subdet][2] < rightbin: maxlimit["phi"+alpha+subdet][2] = rightbin
  for size in [1, 0]:
    for phi in [0, 1]:
      for alpha in ["Mean", "RMS"]:
        hphi[subdet][size][phi][alpha][runs[-1]].GetXaxis().SetRangeUser(maxlimit["phi"+alpha+subdet][1], maxlimit["phi"+alpha+subdet][2])
        hphi[subdet][size][phi][alpha][runs[-1]].GetYaxis().SetRangeUser(0, maxlimit["phi"+alpha+subdet][0]*1.3)

for alpha in ["Mean", "RMS"]:
  c.append(ROOT.TCanvas( 'c'+str(len(c)+1), 'c'+str(len(c)+1), 800, 800 ))
  c[-1].Divide(2,2)

  ##### Draw histograms per SiPM size
  i=1
  for subdet in ["HE", "HB"]:
    for size in [1, 0]:
      c[-1].cd(i)
      for j,run in enumerate(reversed(runs)):
        h[subdet][size][alpha][run].SetLineColor(j+1)
        sizename = "Large" if size==1 else "Small"
        label = subdet + " " + sizename + " SiPM Pedestal "+alpha
        if j==0:
          h[subdet][size][alpha][run].GetXaxis().SetTitle("Pedestal "+alpha+" (QIE11 ADC)")
          h[subdet][size][alpha][run].SetTitle(label)
          h[subdet][size][alpha][run].Draw()
        else:
          h[subdet][size][alpha][run].Draw("same")
      legend.append(ROOT.TLegend(0.1,0.8,0.9,0.9))
      legend[-1].SetNColumns(2)
      for run in runs:
        legend[-1].AddEntry(h[subdet][size][alpha][run],"Run from "+GetDay(run)+", Mean={:.3f}".format(h[subdet][size][alpha][run].GetMean()),"l")
      legend[-1].Draw()
      i += 1
  c[-1].Draw()
  c[-1].SaveAs(output+"PedestalPerSize_"+alpha+".png")
  c[-1].SaveAs(output+"PedestalPerSize_"+alpha+".pdf")

  c.append(ROOT.TCanvas( 'c'+str(len(c)+1), 'c'+str(len(c)+1), 800, 800 ))
  c[-1].Divide(2,2)


  ##### Draw histograms per depth
  for subdet in ["HE", "HB"]:
    if subdet=="HE":
      c.append(ROOT.TCanvas( 'c'+str(len(c)+1), 'c'+str(len(c)+1), 1600, 800 ))
      c[-1].Divide(4,2)
    else:
      c.append(ROOT.TCanvas( 'c'+str(len(c)+1), 'c'+str(len(c)+1), 800, 800 ))
      c[-1].Divide(2,2)
    i=1
    for depth in [1, 2, 3, 4, 5, 6, 7]:
      if subdet=="HB" and depth>4: continue
      c[-1].cd(i)
      for j,run in enumerate(reversed(runs)):
        hdepth[subdet][depth][alpha][run].SetLineColor(j+1)
        label = subdet + " depth " + str(depth) + " Pedestal "+alpha
        if j==0:
          hdepth[subdet][depth][alpha][run].GetXaxis().SetTitle("Pedestal "+alpha+" (QIE11 ADC)")
          hdepth[subdet][depth][alpha][run].SetTitle(label)
          hdepth[subdet][depth][alpha][run].Draw()
        else:
          hdepth[subdet][depth][alpha][run].Draw("same")
      legend.append(ROOT.TLegend(0.1,0.8,0.9,0.9))
      legend[-1].SetNColumns(2)
      for run in runs:
        legend[-1].AddEntry(hdepth[subdet][depth][alpha][run],"Run from "+GetDay(run)+", Mean={:.3f}".format(hdepth[subdet][depth][alpha][run].GetMean()),"l")
      legend[-1].Draw()
      i += 1
    c[-1].Draw()
    c[-1].SaveAs(output+"PedestalPerDepth_"+alpha+"_"+subdet+".png")
    c[-1].SaveAs(output+"PedestalPerDepth_"+alpha+"_"+subdet+".pdf")

  ##### Draw histograms per phi
  for subdet in ["HE", "HB"]:
    c.append(ROOT.TCanvas( 'c'+str(len(c)+1), 'c'+str(len(c)+1), 800, 800 ))
    c[-1].Divide(2,2)
    i=1
    for size in [0, 1]:
      for phi in [0, 1]:
        c[-1].cd(i)
        for j,run in enumerate(reversed(runs)):
          hphi[subdet][size][phi][alpha][run].SetLineColor(j+1)
          sizename = "Large" if size==1 else "Small"
          phirange = "for iphi in [35,36,37,38] " if phi==0 else "for iphi in [11,12,13,14] "
          label = subdet + " " + sizename + " SiPM Pedestal " + phirange +alpha
          if j==0:
            hphi[subdet][size][phi][alpha][run].GetXaxis().SetTitle("Pedestal "+alpha+" (QIE11 ADC)")
            hphi[subdet][size][phi][alpha][run].SetTitle(label)
            hphi[subdet][size][phi][alpha][run].Draw()
          else:
            hphi[subdet][size][phi][alpha][run].Draw("same")
        legend.append(ROOT.TLegend(0.1,0.8,0.9,0.9))
        legend[-1].SetNColumns(2)
        for run in runs:
          legend[-1].AddEntry(hphi[subdet][size][phi][alpha][run],"Run from "+GetDay(run)+", Mean={:.3f}".format(hphi[subdet][size][phi][alpha][run].GetMean()),"l")
        legend[-1].Draw()
        i += 1
    c[-1].Draw()
    c[-1].SaveAs(output+"PedestalPerPhi_"+alpha+"_"+subdet+".png")
    c[-1].SaveAs(output+"PedestalPerPhi_"+alpha+"_"+subdet+".pdf")
