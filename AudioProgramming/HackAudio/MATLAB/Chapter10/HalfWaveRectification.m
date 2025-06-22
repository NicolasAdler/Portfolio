function out = HalfWaveRectification(in)
N = length(in);
out = zeros(N,1);
    for n = 1:N
        if in(n,1) >= 0
            out(n,1) = in(n,1);
        else
            out(n,1) = 0;
        end
    end
end
