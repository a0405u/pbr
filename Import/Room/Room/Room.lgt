;; Created by Lumicept

Geography TokyoMidday
  zenith 0.000000 1.000000 0.000000
  south 1.000000 0.000000 0.000000
  latitude 36.000000
  longitude -140.000000
  month 6
  date 21
  time 12:0
  GMT 9
  DayLightSaving off
enddef

Light L000001
  type  point
  TotalFlux 760
  MF 1.000000
  intensity 1.000000 1.000000 1.000000
  CastShadows on
enddef

Light L000009
  type  point
  TotalFlux 300
  MF 1.000000
  intensity 0.781200 0.859400 1.000000
  CastShadows on
enddef

Light L000008
  type  point
  TotalFlux 300
  MF 1.000000
  intensity 0.780400 0.858800 1.000000
  CastShadows on
enddef

Fixture L000001
  Light L000001
  Switch on
  Sharp on
  Position  -6.58810   1.52170  -6.68720
  Direction 0.000000 -1.000000 -0.000000
enddef


Fixture L000002
  Light L000001
  Switch on
  Sharp on
  Position  -11.9019   1.52170  -6.77130
  Direction 0.000000 -1.000000 -0.000000
enddef


Fixture L000003
  Light L000001
  Switch on
  Sharp on
  Position  -9.22390   2.13160  -7.36720
  Direction 0.000000 -1.000000 -0.000000
enddef


Fixture L000004
  Light L000001
  Switch on
  Sharp on
  Position  -7.54850   1.52170  -7.90700
  Direction 0.000000 -1.000000 -0.000000
enddef


Fixture L000005
  Light L000001
  Switch on
  Sharp on
  Position  -7.70270   1.52170  -10.3185
  Direction 0.000000 -1.000000 -0.000000
enddef


Fixture L000006
  Light L000001
  Switch on
  Sharp on
  Position  -10.7732   1.52170  -10.3185
  Direction 0.000000 -1.000000 -0.000000
enddef


Fixture L000007
  Light L000001
  Switch on
  Sharp on
  Position  -10.8503   1.52170  -7.93500
  Direction 0.000000 -1.000000 -0.000000
enddef


Fixture L000009
  Light L000009
  Switch on
  Sharp on
  Position  -12.0833   1.85000  -16.4666
  Direction 0.000000 -1.000000 -0.000000
enddef


Fixture L000008
  Light L000008
  Switch on
  Sharp on
  Position  -6.91730   2.15000  -16.5333
  Direction 0.000000 -1.000000 -0.000000
enddef


Group main
  Switch on
  Edit   on
  Shown  on
  Power  100.0%
  def
          L000001
          L000002
          L000003
          L000004
          L000005
          L000006
          L000007
          L000009
          L000008
enddef


