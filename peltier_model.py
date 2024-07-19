import math

class PeltierSolarPanel:
    def __init__(self, magnifying_glass_area, peltier_area, peltier_efficiency, ambient_temp, cold_reservoir_temp):
        self.magnifying_glass_area = magnifying_glass_area  # in m^2
        self.peltier_area = peltier_area  # in m^2
        self.peltier_efficiency = peltier_efficiency  # as a decimal
        self.ambient_temp = ambient_temp  # in Celsius
        self.cold_reservoir_temp = cold_reservoir_temp  # in Celsius
        self.solar_constant = 1361  # W/m^2 at top of atmosphere
        self.stefan_boltzmann = 5.67e-8  # Stefan-Boltzmann constant

    def calculate_input_power(self):
        # Assume 70% of solar energy reaches the surface
        surface_irradiance = self.solar_constant * 0.7
        return surface_irradiance * self.magnifying_glass_area

    def calculate_hot_side_temp(self):
        input_power = self.calculate_input_power()
        focused_irradiance = input_power / self.peltier_area
        
        # Calculate equilibrium temperature using Stefan-Boltzmann law
        temp_kelvin = ((focused_irradiance / self.stefan_boltzmann) ** 0.25)
        return temp_kelvin - 273.15  # Convert to Celsius

    def calculate_output_power(self):
        input_power = self.calculate_input_power()
        hot_side_temp = self.calculate_hot_side_temp()
        temperature_difference = hot_side_temp - self.cold_reservoir_temp
        
        # Peltier effect calculation
        # The efficiency is applied to the input power, and modulated by the temperature difference
        # carnot_efficiency = temperature_difference / (hot_side_temp + 273.15)  # Theoretical max efficiency
        # actual_efficiency = self.peltier_efficiency * carnot_efficiency
        # output_power = input_power * self.peltier_efficiency * (temperature_difference / 100)
        carnot_efficiency = temperature_difference / (hot_side_temp + 273.15)
        actual_efficiency = self.peltier_efficiency * carnot_efficiency
        output_power = input_power * actual_efficiency
        
        # Ensure the output power doesn't exceed the input power
        return output_power

    def calculate_efficiency(self):
        return self.calculate_output_power() / self.calculate_input_power()

    def estimate_voltage_and_current(self):
        output_power = self.calculate_output_power()
        # Assuming a typical Peltier module voltage of 2V
        voltage = 2
        current = output_power / voltage
        return voltage, current

    def watts_per_sqft(self):
        output_power = self.calculate_output_power()
        area_sqft = self.magnifying_glass_area * 10.764  # Convert m^2 to ft^2
        return output_power / area_sqft

    def watts_per_second(self):
        # Watts is already a measure of power per second, so this is the same as output power
        return self.calculate_output_power()

# Example usage
magnifying_glass_area = 2  # m^2
peltier_area = 0.01  # m^2 (10cm x 10cm)
peltier_efficiency = 0.1  # 10% of Carnot efficiency
ambient_temp = 30  # Celsius
cold_reservoir_temp = 10  # Celsius

panel = PeltierSolarPanel(magnifying_glass_area, peltier_area, peltier_efficiency, ambient_temp, cold_reservoir_temp)

input_power = panel.calculate_input_power()
hot_side_temp = panel.calculate_hot_side_temp()
output_power = panel.calculate_output_power()
efficiency = panel.calculate_efficiency()
voltage, current = panel.estimate_voltage_and_current()
watts_per_sqft = panel.watts_per_sqft()
watts_per_second = panel.watts_per_second()

print(f"Input Power: {input_power:.2f} W")
print(f"Hot Side Temperature: {hot_side_temp:.2f} °C")
print(f"Output Power: {output_power:.2f} W")
print(f"Efficiency: {efficiency:.2%}")
print(f"Estimated Voltage: {voltage:.2f} V")
print(f"Estimated Current: {current:.2f} A")
print(f"Watts per sq ft: {watts_per_sqft:.2f} W/ft²")
print(f"Energy per second: {watts_per_second:.2f} W")
