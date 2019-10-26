#include<stdio.h>

double getPi(int n);
int factorial(int x);
double power(double x, int times);
double ln(double x, int n);
double exp(double x, long n);

int main(){
    double x, ref, pi, lnPi, piX;
    FILE *file = fopen("data.txt", "r");
    while(fscanf(file, "%lf %lf", &x, &ref) == 2){
        pi = getPi(1000000);
        lnPi = 4 * ln(1.25, 24) + ln(0.4096 * pi, 24);
        piX = exp(lnPi * x, 1e7);
        printf("x:\t%.14lf\npiX:\t%.14lf\t%.6lf\nref:\t%.14lf\t%.6lf"
                "\n\n", x, piX, piX, ref, ref);
    }
    fclose(file);
    return 0;
}

double f1(double x){
    return 1 / (x * x + 1);
}

double getPi(int n){
    double h = 1 / (double)n;
    double sum = 1.5;
    for(int i = 1; i < n; i++){
        sum += f1(h * i) * 2;
    }
    return sum * h * 2;
}

int factorial(int x){
    int result = 1;
    for(int i = 2; i <= x; ++i)
        result *= i;
    return result;
}

double power(double x, int times){
    double result = 1;
    for(int i = 1; i <= times; ++i)
        result *= x;
    return result;
}

double ln(double x, int n){
    x = x - 1;
    double sum = 0;
    for(int i = 1; i <= n; ++i){
        sum += power(x, i) / i * (i % 2 ? 1 : -1);
    }
    return sum;
}

double exp(double x, long n){
    __float128 h = x / n;
    __float128 factor = 1 + h + h * h / 2;
    __float128 result = 1;
    for(int i = 0; i < n; i++)
        result *= factor;
    return result;
}
