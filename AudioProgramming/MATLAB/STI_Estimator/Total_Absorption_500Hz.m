classdef Total_Absorption_500Hz
    %{
     Given the number of surfaces and the absorption coefficients at 500 Hz for those respective surfaces,
     This class will calculate the total absorption of a room at 500 Hz.
    %}
    
    properties
        number_of_surfaces
        surface_areas
        absorption_coefficients
    end
    
    methods
        function obj = Total_Absorption_500Hz(num_surfaces, surface_areas, absorption_coefficients)
            obj.number_of_surfaces = num_surfaces;
            obj.surface_areas = surface_areas;
            obj.absorption_coefficients = absorption_coefficients;
        end
        
        function total_absorption_value = Calculate_Total_Absorption_500Hz(obj)
            total_absorption_value = sum(obj.surface_areas .* obj.absorption_coefficients);
        end
    end
end
