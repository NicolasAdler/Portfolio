classdef RT60_Calculator
    %{ 
    %  This class can be used in conjunction with "Total_Absorption_500" to
    %  get the RT60 at 500 Hz. Given the volume and the total absorption of
    %  the room
    %}
    
    properties
        volume
        total_absorbtion
    end
    
    methods
        function obj = RT60_Calculator(volume, total_absorbtion)
            obj.volume = volume;
            obj.total_absorbtion = total_absorbtion;
        end
        
        function RT60 = calculate_RT60(obj)
            % RT60 = 0.161 * obj.volume / obj.total_absorbtion;
            RT60 = 0.049 * obj.volume / obj.total_absorbtion;
           
        end
    end
end

