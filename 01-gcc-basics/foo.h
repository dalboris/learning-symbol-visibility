#pragma once

int add(int a, int b);

__attribute__ ((visibility ("default")))
int sub(int a, int b);

__attribute__ ((visibility ("hidden")))
int mult(int a, int b);
