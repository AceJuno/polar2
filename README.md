# polarImageProcessing

This C++ library provides few algorithms that allow polarized image processing.
Implemented algorithms include:
- DoFP demosiacing using Newton's Polynomial interpolation and polarization difference model [1]
- Stokes Vector algorithm (S0, S1, S2) calculated from demosaiced image
- AoLP - angle of linear polarization image
- DoLP - degree of linear polarization image
- AoLPColor - same as AoLP but with a HSV color mapped to angles of polarization
- Mueller Matrix - calculated from sufficient sample size(4x4) [2]

This project uses OpenCV library.

[1] https://opg.optica.org/oe/fulltext.cfm?uri=oe-27-2-1376&id=404023

[2] https://www.sciencedirect.com/topics/engineering/mueller-matrix
