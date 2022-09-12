# -*- coding: utf-8 -*-
import math
pi=math.pi
#constants--------------------------------------------------
L= 0.285 #meters length of his spokes
Density= 7700 #kg/m^3; #Density of Steel
r= 0.9E-3  #radius of his spoke
Volume= pi*r**2*L #volume calculations
Mass=Density*Volume #mass calculations
mu= Mass/L #mass per unit length
E= 200E9 #N/m^2 - Young's Modulus
kappa=r/2 #variable used in the tension formula
s=pi*r**2 #cross sectional area
n=1 #nth harmonic
alpha=(4 + (n**2*pi**2/2))#simplifed constant for the tension formula


def TensionEquation(T, freq): #will take in the frequency values as freq
    beta=(E*s*kappa**2/T)**(1/2) #constant
    #full equation paramterized
    return ( (1/(2*L))*(T/mu)**(1/2)*(1+ (2*beta/L) + (alpha*beta**2/L**2))-freq)

