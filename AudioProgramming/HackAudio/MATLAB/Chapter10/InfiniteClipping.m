function out = InfiniteClipping(in)
N = length(in);
out = zeros(N,1);
    for n = 1:N
        if n >= 0
            out(n,1) = 1;
        else
            out(n,1) = -1;
        end
    
    end

end