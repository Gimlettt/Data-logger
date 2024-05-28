int analogPin = A0; // Pin where the signal is read
float alpha = 0.1; // Smoothing factor (0 < alpha < 1)
int previousValue = 0;

void readFilteredADC() {
    int currentValue = analogRead(analogPin);
    int filteredValue = (alpha * currentValue) + ((1 - alpha) * previousValue);
    previousValue = filteredValue;
    return filteredValue;
}
