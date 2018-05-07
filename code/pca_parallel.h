#ifndef PCAPARALLEL_H_
#define PCAPARALLEL_H_
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float *computeMeanVector(float **data, int amountOfElements, int dimension);
float **calculateCovarianceMatrix(float **data, int amountOfElements, int dimension, float *meanVectors);
void getCovarianceMatrix(float **dataTranspose, float **data, float *meanVectors, int dimension, int amountOfElements, float **covarianceMatrix);
float **computeEigenValues(float **covarianceMatrix, int dimension, int dimensionToMapTo);
float **transformSamplesToNewSubspace(float **allData, float **eigenVectorMatrix, int r1, int c1, int r2, int c2);
float **matrixMultiplication(float **matrixA, float **matrixB, int r1, int c1, int r2, int c2);
void pca_parallel(int amountOfElements, int dimension, int dimensionToMapTo)
{
    FILE *fp = fopen("../data/data.txt", "r");
    if (fp == NULL)
    {
        perror("Error in opening file");
        exit(EXIT_FAILURE);
    }

    float **allData = (float **)malloc(sizeof(float *) * amountOfElements);
    for (int i = 0; i < amountOfElements; i++)
    {
        allData[i] = (float *)malloc(sizeof(float) * dimension);
    }

    loadAllData(allData, fp);

    printf("\nInitial Data \n");
    printAllData(allData, amountOfElements, dimension);

    printf("\nTimes taken for %d elements dimension of %d, mapping to %d principal compoments. \n",amountOfElements,dimension,dimensionToMapTo);
    clock_t beginMV = clock();
    float *meanVectors = computeMeanVector(allData, amountOfElements, dimension);
    clock_t endMV = clock();
    double time_spentMV = (double)(endMV - beginMV) / CLOCKS_PER_SEC;
    printf("Time to calculate mean vector: %f \n", time_spentMV);

    clock_t beginCV = clock();
    float **covarianceMatrix = calculateCovarianceMatrix(allData, amountOfElements, dimension, meanVectors);
    clock_t endCV = clock();
    double time_spentCV = (double)(endCV - beginCV) / CLOCKS_PER_SEC;
    printf("Time to calculate co variance matrix: %f \n", time_spentCV);

    clock_t beginEV = clock();
    float **eigenVectorMatrix = computeEigenValues(covarianceMatrix, dimension, dimensionToMapTo);
    clock_t endEV = clock();
    double time_spentEV = (double)(endEV - beginEV) / CLOCKS_PER_SEC;
    printf("Time to calculate eigenvalues: %f \n", time_spentEV);

    clock_t beginNV = clock();
    float **newValues = transformSamplesToNewSubspace(allData, eigenVectorMatrix, amountOfElements, dimension, dimension, dimensionToMapTo);
    clock_t endNV = clock();
    double time_spentNV = (double)(endNV - beginNV) / CLOCKS_PER_SEC;
    printf("Time to map inputs to %d principal components: %f \n", dimensionToMapTo, time_spentNV);

    printf("\nData after PCA to %d principal components \n", dimensionToMapTo);
    //printAllData(newValues, amountOfElements, dimensionToMapTo);
}

float *computeMeanVector(float **data, int amountOfElements, int dimension)
{
    float *meanVectors = (float *)malloc(sizeof(float) * dimension);
    for (int j = 0; j < dimension; j++)
    {
        float mean = 0.0;
        float sum = 0.0;

        for (int i = 0; i < amountOfElements; i++)
        {
            sum += data[i][j];
        }
        meanVectors[j] = sum / amountOfElements;
    }
    return meanVectors;
}

float **calculateCovarianceMatrix(float **data, int amountOfElements, int dimension, float *meanVectors)
{
    float **covarianceMatrix = (float **)malloc(sizeof(float *) * dimension);
    for (int i = 0; i < dimension; i++)
    {
        covarianceMatrix[i] = (float *)malloc(sizeof(float) * dimension);
    }
    float **dataTranspose = (float **)malloc(sizeof(float *) * dimension);
    for (int i = 0; i < dimension; i++)
    {
        dataTranspose[i] = (float *)malloc(sizeof(float) * amountOfElements);
    }
    for (int j = 0; j < dimension; j++)
    {
        for (int i = 0; i < amountOfElements; i++)
        {
            dataTranspose[j][i] = data[i][j];
        }
    }

    getCovarianceMatrix(dataTranspose, data, meanVectors, dimension, amountOfElements, covarianceMatrix);
    return covarianceMatrix;
}

void getCovarianceMatrix(float **dataTranspose, float **data, float *meanVectors, int dimension, int amountOfElements, float **covarianceMatrix)
{
    float sum;
    int r1 = dimension;
    int c2 = dimension;
    int c1 = amountOfElements;
    for (int i = 0; i < r1; ++i)
    {
        for (int j = 0; j < c2; ++j)
        {
            sum = 0.0;
            for (int k = 0; k < c1; ++k)
            {
                sum += (dataTranspose[i][k] - meanVectors[j]) * (data[k][j] - meanVectors[j]);
            }
            covarianceMatrix[i][j] = sum / (amountOfElements - 1);
        }
    }
}

float **computeEigenValues(float **covarianceMatrix, int dimension, int dimensionToMapTo)
{
    float **eigenVectorMatrix;
    return eigenVectorMatrix;
}

float **transformSamplesToNewSubspace(float **allData, float **eigenVectorMatrix, int r1, int c1, int r2, int c2)
{
    float **newValues;//= matrixMultiplication(allData, eigenVectorMatrix, r1, c1, r2, c2);
    return newValues;
}

float **matrixMultiplication(float **matrixA, float **matrixB, int r1, int c1, int r2, int c2)
{
    float **result = (float **)malloc(sizeof(float *) * r1);
    float sum;
    for (int i = 0; i < r1; i++)
    {
        result[i] = (float *)malloc(sizeof(float) * c2);
    }
    for (int i = 0; i < r1; ++i)
    {
        for (int j = 0; j < c2; ++j)
        {
            sum = 0.0;
            for (int k = 0; k < c1; ++k)
            {
                sum += (matrixA[i][k]) * (matrixB[k][j]);
            }
            result[i][j] = sum;
        }
    }
    return result;
}
#endif