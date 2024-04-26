#pragma once

double rand01();
bool   pileFace(double p);
double genDirection();

float loiExp(float lambda);

float loiBeta(float alpha, float beta);
float loiCauchy(float alpha, float x);
int   loiBinomiale(int tries, float p);
int   loiBernoulli(int p);
int   loiUniforme(int min, int max);