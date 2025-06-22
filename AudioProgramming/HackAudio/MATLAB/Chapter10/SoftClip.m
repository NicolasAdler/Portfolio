function out = SoftClip(in, a, type)
N = length(in);
out = zeros(N,1);
    for n = 1:N
        if type == "Cubic"
            out(n,1) = in(n,1) - a*(1/3) * (in(n,1).^3); 
        elseif type == "Arctan"
            out(n,1) = (2/pi)*atan(in(n,1)*a);
        end
    end
end