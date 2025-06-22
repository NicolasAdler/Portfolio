classdef STI_Calculator
    % Given the RT60 at 500 Hz of a room, this class calculates an estimate
    % STI of a room.
    
    properties
        RT60
    end
    
    methods
        function obj = STI_Calculator(RT60)
            obj.RT60 = RT60;
        end
        
        function STI = Calculate_STI(obj)
            STI_1 = 0.5895 - 0.4422 * log10(obj.RT60);
            STI_2 = 0.778 - (0.143 * obj.RT60);
            STI_3 = 0.634-0.192 * log(obj.RT60);
            STI_4 = 0.6488 - 0.2078 * log(obj.RT60);
            STI = round((STI_1 + STI_2 + STI_3 + STI_4) / 4, 2); 
            if STI >= 0.75
                disp(['STI is Excellent, it is calculated at: ', num2str(STI)]);
            elseif STI >= 0.6
                disp(['STI is Good, it is calculated at: ', num2str(STI)]);
            elseif STI >= 0.45
                disp(['STI is Fair, it is calculated at: ', num2str(STI)]);
            elseif STI >= 0.3
                disp(['STI is Poor, it is calculated at: ', num2str(STI)]);
            else 
                disp(['STI is Bad, it is calculated at: ', num2str(STI)]);
            end
        end
    end
end

