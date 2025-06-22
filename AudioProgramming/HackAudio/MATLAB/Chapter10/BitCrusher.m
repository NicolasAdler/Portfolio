function out = BitCrusher(in,nBits)
   in = in(:);
   ampValues = 2 ^ nBits;
   prepInput = 0.5*in + 0.5;
   scaleInput = ampValues * prepInput;
   roundInput = round(scaleInput);
   prepOut = roundInput / ampValues;
   out = 2*prepOut - 1;
end