# fan-speed-control
Arduino speed control for a four-wire desktop computer fan. Good for magnetic stirrers.

## Fan controls
Four-wire computer fans have speed controlled by PWM applied to one of the
wires. The wire is normally pulled up to 5-12 V, and to reduce the fan speed,
we should pull it down. Pulling it down all the time would tell the fan to turn
at its lowest speed. Leaving it high lets the fan run at max speed. If we use
PWM to pull it down some of the time, the duty cycle will determine the fan's
speed. Note that you must use a transistor to pull the connection down, as the
voltage sometimes goes much higher than an Anduino port can handle.

The fan may not be able to handle very high frequency PWM, so Arduino is
advantageous over a faster dedicated PWM controller.

## Usage
My circuit has the optional feature of using a momentary push button to change
whether the fan is on. Connect one end of the button to Arduino and the other
end to ground, so the button press pulls the pin down. If you don't need this,
you can hook the fan up to a 12 V power supply directly.

I am using a 10 k potentiometer (as a voltage divider) to control the speed.
Measure voltage from the pot's middle pin. If you want to use some other logic
to determine the desired speed, rewrite the `getChosenSpeed` function.

My circuit uses two 2222A transistors, but you could use almost any transistor.

## Why not use a 555 timer?
Since Arduino is popular with beginners, there are a lot of people that don't
have a 555 timer. If you have it and know how to use it, use the 555! You may
need to limit the frequency so it doesn't get so high the fan gets confused.

## Use for magnetic stirrers
Magnetic stirrers are very easy to make with a computer fan, but people
typically control their speed by limiting the voltage with a variable
resistor. This works badly because they may not have enough torque to get
started until they're at nearly full power. (And good luck trying to stir a thick
liquid.) Using the built in speed control doesn't have that problem. However,
operation is still limited by the lowest speed. My fan's minimum speed is
probably still too fast to stir something very thick like honey.

## Circuit
![Circuit diagram](fan-speed-control.png?raw=true)

## More info about 4-wire fans
I learned about the speed control in this video:
https://www.youtube.com/watch?v=gKHww3qJbs8
