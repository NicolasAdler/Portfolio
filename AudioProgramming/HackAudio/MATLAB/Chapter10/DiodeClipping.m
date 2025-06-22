function out = DiodeClipping(in)

Vt = 0.0253; % thermal voltage
eta = 1.68; % emission coefficient
Is = .105;  % saturation current

N = length(in);
out = zeros(N,1);
    for n = 1:N
        %out(n,1) = Is * ((exp((0.1 * in(n,1))/(eta *Vt))-1);
        out(n,1) = Is * (exp(0.1*in(n,1)/(eta*Vt)) - 1);
    end
end
