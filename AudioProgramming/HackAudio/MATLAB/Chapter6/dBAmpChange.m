function [ out ] = dbAmpChange( in, dBChange )
    scale = 10^(dBChange/20);
    out = scale*in;
end
