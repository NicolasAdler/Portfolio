function out = PieceWiseOverdrive(in)
N = length(in);
out = zeros(N,1);
    for n = 1:N
        if abs(in(n,1)) <= 1/3
            out(n,1) = 2*in(n,1); 
        elseif abs(in(n,1)) > 2/3
            out(n,1) = sign(in(n,1));
        else
        end
    end
end