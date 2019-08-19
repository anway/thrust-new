// main06.cc is a part of the PYTHIA event generator.
// Copyright (C) 2018 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// It studies event properties of LEP1 events.

#include <cstdio>
#include <ctime>
#include "Pythia8/Pythia.h"

using namespace Pythia8;

int main() {

  // Generator.
  Pythia pythia;

  // Allow no substructure in e+- beams: normal for corrected LEP data.
  pythia.readString("PDF:lepton = off");
  // Process selection.
  pythia.readString("WeakSingleBoson:ffbar2gmZ = on");
  // Switch off all Z0 decays and then switch back on those to quarks.
  pythia.readString("23:onMode = off");
  pythia.readString("23:onIfAny = 1 2 3 4 5");

  // LEP1 initialization at Z0 mass.
  pythia.readString("Beams:idA =  11");
  pythia.readString("Beams:idB = -11");
  double mZ = pythia.particleData.m0(23);
  pythia.settings.parm("Beams:eCM", mZ);
  pythia.init();

  // Histogram for thrust difference.
  Hist dThrust("delta thrust", 100, -1., 1.);
  Hist dOblateness("delta oblateness", 100, -1., 1.);
  Hist dTAxis("delta cos(theta_Thrust)", 100, -2., 2.);
  Hist dTime("delta time", 100, -2., 2.);

  // Set up Thrust analysis.
  Thrust thrOld, thrNew;

  // Begin event loop. Generate event. Skip if error. List first few.
  for (int iEvent = 0; iEvent < 10000; ++iEvent) {
    if (!pythia.next()) continue;

    // Find and histogram thrust.
    std::clock_t thrOldStart = std::clock();
    bool thrOldDone = thrOld.analyze( pythia.event );
    double thrOldEnd = std::clock() - thrOldStart;
    if (thrOldDone) {
		std::clock_t thrNewStart = std::clock();
        bool thrNewDone = thrNew.analyze( pythia.event );
		double thrNewEnd = std::clock() - thrNewStart;
        if (thrNewDone) {
          if (iEvent < 3) {
			cout << "Old thrust result" << endl;
			thrOld.list();
			cout << "New thrust result" << endl;
            thrNew.list(); }
          dThrust.fill( thrNew.thrust() - thrOld.thrust());
          dOblateness.fill( thrNew.oblateness() - thrOld.oblateness());
          dTAxis.fill( thrNew.eventAxis(1).pz() - thrOld.eventAxis(1).pz());
          dTime.fill( thrOldEnd - thrNewEnd);
      }
    }

  // End of event loop. Statistics. Output histograms.
  }
  pythia.stat();
  cout << dThrust << dOblateness << dTAxis << dTime << endl;;

  // Done.
  return 0;
}
