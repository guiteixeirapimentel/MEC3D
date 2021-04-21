#pragma once
#include <iostream>
#include <vector>

#include "MalhaTriangular.h"
#include "Triangulo.h"
#include "Matriz.h"

void MostrarMatriz(const Matriz& m, const char* label);

std::pair<std::vector<InstElemento>, std::vector<Ponto>> TriforceTesselation(const std::vector<InstElemento>& elementos,
    const std::vector<Ponto>& nodes, int nivel);

std::pair<std::vector<CondContorno>, std::vector<CondContorno>> CorrigeCondContornoTess(const std::vector<CondContorno>& deslPresc,
    const std::vector<CondContorno>& forcsPrescs, int nivel);


void MostrarMatriz(const Matriz& m, const char* label)
{
    std::cout << label << " = { " << std::endl;
    for (int i = 0; i < m.GetNLinhas(); i++)
    {
        for (int j = 0; j < m.GetNColunas(); j++)
        {
            std::cout << m(i, j);
            if (j != m.GetNColunas() - 1)
                std::cout << "; ";
        }
        std::cout << "\n";
    }

    std::cout << "};" << std::endl;
}


std::pair<std::vector<InstElemento>, std::vector<Ponto>> TriforceTesselation(const std::vector<InstElemento>& elementos,
    const std::vector<Ponto>& nodes, int nivel)
{
    if (nivel == 0)
    {
        return { elementos, nodes };
    }

    const int nne = elementos.size();
    const int nno = nodes.size();

    std::vector<InstElemento> eleTesselated;
    eleTesselated.resize(4 * nne, {});
    std::vector<Ponto> nodesTesselated;
    nodesTesselated.resize(4 * nno, {});

    int newEles = 0;
    int newNodes = 0;

    for (int ie = 0; ie < nne; ie++)
    {
        int i1 = elementos[ie].idPonto1;
        int i2 = elementos[ie].idPonto2;
        int i3 = elementos[ie].idPonto3;

        Ponto X1 = nodes[i1 - 1];

        Ponto X2 = nodes[i2 - 1];

        Ponto X3 = nodes[i3 - 1];

        Ponto X4 = ((Vetor(X1) * 1.0) + X2) / 2;
        Ponto X5 = ((Vetor(X2) * 1.0) + X3) / 2;
        Ponto X6 = ((Vetor(X3) * 1.0) + X1) / 2;

        nodesTesselated[newNodes] = X1;
        nodesTesselated[newNodes + 1] = X4;
        nodesTesselated[newNodes + 2] = X6;

        eleTesselated[newEles] = { newNodes + 1, newNodes + 2, newNodes + 3 };
        newEles = newEles + 1;

        newNodes = newNodes + 3;

        nodesTesselated[newNodes] = X4;
        nodesTesselated[newNodes + 1] = X2;
        nodesTesselated[newNodes + 2] = X5;

        eleTesselated[newEles] = { newNodes + 1, newNodes + 2, newNodes + 3 };
        newEles = newEles + 1;

        newNodes = newNodes + 3;

        nodesTesselated[newNodes] = X4;
        nodesTesselated[newNodes + 1] = X5;
        nodesTesselated[newNodes + 2] = X6;

        eleTesselated[newEles] = { newNodes + 1, newNodes + 2, newNodes + 3 };
        newEles = newEles + 1;

        newNodes = newNodes + 3;

        nodesTesselated[newNodes] = X5;
        nodesTesselated[newNodes + 1] = X3;
        nodesTesselated[newNodes + 2] = X6;

        eleTesselated[newEles] = { newNodes + 1, newNodes + 2, newNodes + 3 };
        newEles = newEles + 1;

        newNodes = newNodes + 3;
    }


    return  TriforceTesselation(eleTesselated, nodesTesselated, nivel - 1);

}

std::pair<std::vector<CondContorno>, std::vector<CondContorno>> CorrigeCondContornoTess(const std::vector<CondContorno>& ldp,
    const std::vector<CondContorno>& lfp, int nivel)
{
    if (nivel == 0)
    {
        return { ldp, lfp };
    }

    const int ndp = ldp.size();
    const int nfp = lfp.size();

    std::vector<CondContorno> ldptess;
    ldptess.resize(4 * ndp, {});
    std::vector<CondContorno> lfptess;
    lfptess.resize(4 * nfp, {});

    int indp = 0;

    for (int idp = 0; idp < ndp; idp++)
    {
        int newildp = (ldp[idp].idEle * 4) - 3;

        ldptess[indp] = { newildp, ldp[idp].grauLib, ldp[idp].val };
        ldptess[indp + 1] = { newildp + 1, ldp[idp].grauLib, ldp[idp].val };
        ldptess[indp + 2] = { newildp + 2, ldp[idp].grauLib, ldp[idp].val };
        ldptess[indp + 3] = { newildp + 3, ldp[idp].grauLib, ldp[idp].val };

        indp = indp + 4;
    }


    int infp = 0;

    for (int ifp = 0; ifp < nfp; ifp++)
    {
        int newilfp = (lfp[ifp].idEle * 4) - 3;

        lfptess[infp] = { newilfp, lfp[ifp].grauLib, lfp[ifp].val };
        lfptess[infp + 1] = { newilfp + 1, lfp[ifp].grauLib, lfp[ifp].val };
        lfptess[infp + 2] = { newilfp + 2, lfp[ifp].grauLib, lfp[ifp].val };
        lfptess[infp + 3] = { newilfp + 3, lfp[ifp].grauLib, lfp[ifp].val };

        infp = infp + 4;
    }

    return CorrigeCondContornoTess(ldptess, lfptess, nivel - 1);
}