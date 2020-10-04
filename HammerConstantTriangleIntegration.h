#pragma once
#include "Matriz.h"
#include "Triangulo.h"
#include "FundSolPStar.h"
#include "FundSolUStar.h"

class HammerConstantTriangleIntegration
{
public:
    HammerConstantTriangleIntegration() {};
    ~HammerConstantTriangleIntegration() {};

    double Integrate7PtsUStar(const Triangulo& triangulo, const Vetor& ksi, 
        const FundSolUStar& func, int i, int j) const
    {
        double res = 0.0;

        for (size_t ni = 0; ni < cPts7.size(); ni++)
        {
            const Vetor posGlobalX = triangulo.GetGlobalFromHomogeneous(cPts7[ni]);

            res += cWeights7[ni] * func(i, j, ksi, posGlobalX);

        }

        return res * 2.0 * triangulo.cArea;
    }

    double Integrate7PtsPStar(const Triangulo& triangulo, const Vetor& ksi,
        const FundSolPStar& func, int i, int j) const
    {
        double res = 0.0;

        for (size_t ni = 0; ni < cPts7.size(); ni++)
        {
            const Vetor posGlobalX = triangulo.GetGlobalFromHomogeneous(cPts7[ni]);

            res += cWeights7[ni] * func(i, j, ksi, posGlobalX, triangulo.cNormalU);

        }

        return res * 2.0 * triangulo.cArea;
    }

    double Integrate7PtsUStarSubElement(const Triangulo& triangulo, const Vetor& ksi,
        const FundSolUStar& func, int i, int j, int subsInicial = 4, double maxResidual = 1e-8)
    {
        double res = 0.0;
        const int maxSubs = 10;

        int qtdSubs = subsInicial;

        double resm1 = 0.0;
        for (; qtdSubs < maxSubs; qtdSubs++)
        {
            const Vetor P1 = triangulo.cP1;
            const Vetor P2 = triangulo.cP2;
            const Vetor P3 = triangulo.cP3;

            res = IntegrateWithSubHStar(P1, P2, P3, ksi, func, i, j, qtdSubs);

            const double residual = std::fabs(resm1 - res);

            if (residual < maxResidual)
            {
                break;
            }

            resm1 = res;
        }
            

        return res;
    }

    double Integrate7PtsPStarSubElement(const Triangulo& triangulo, const Vetor& ksi,
        const FundSolPStar& func, int i, int j, int subsInicial = 4, double maxResidual = 1e-8) const
    {
        double res = 0.0;
        const int maxSubs = 10;

        int qtdSubs = subsInicial;

        double resm1 = 0.0;
        for (; qtdSubs < maxSubs; qtdSubs++)
        {
            const Vetor P1 = triangulo.cP1;
            const Vetor P2 = triangulo.cP2;
            const Vetor P3 = triangulo.cP3;

            res = IntegrateWithSubPStar(P1, P2, P3, triangulo.cNormalU, ksi, func, i, j, qtdSubs);

            const double residual = std::fabs(resm1 - res);

            if (residual < maxResidual)
            {
                break;
            }

            resm1 = res;
        }


        return res;
    }

private:
    double IntegrateWithSubPStar(const Vetor& P1, const Vetor& P2, const Vetor& P3, const Vetor& normal, const Vetor& ksi,
        const FundSolPStar& func, int i, int j, int nivel) const
    {
        double res = 0.0;

        if (nivel == 0)
        {
            for (size_t ni = 0; ni < cPts7.size(); ni++)
            {
                const Vetor posGlobalX = Triangulo::GetGlobalFromHomogeneous(cPts7[ni], P1, P2, P3);

                res += cWeights7[ni] * func(i, j, ksi, posGlobalX, normal);
            }

            const double A = ((P2-P1).Cross(P3-P1)).Mod() / 2.0;

            return res * 2.0 * A;
        }
        
        else
        {
            const Vetor Pp1 = ((P1 * 1.0) + P2) / 2.0;
            const Vetor Pp2 = ((P2 * 1.0) + P3) / 2.0;
            const Vetor Pp3 = ((P3 * 1.0) + P1) / 2.0;
            
            res += IntegrateWithSubPStar(P1, Pp1, Pp3, normal, ksi,
                func, i, j, nivel - 1);

            res += IntegrateWithSubPStar(Pp1, Pp2, Pp3, normal, ksi,
                func, i, j, nivel - 1);

            res += IntegrateWithSubPStar(Pp1, P2, Pp2, normal, ksi,
                func, i, j, nivel - 1);

            res += IntegrateWithSubPStar(Pp2, P3, Pp3, normal, ksi,
                func, i, j, nivel - 1);
            
            return res;
        }

    }

    double IntegrateWithSubHStar(const Vetor& P1, const Vetor& P2, const Vetor& P3, const Vetor& ksi,
        const FundSolUStar& func, int i, int j, int nivel) const
    {
        double res = 0.0;

        if (nivel == 0)
        {
            for (size_t ni = 0; ni < cPts7.size(); ni++)
            {
                const Vetor posGlobalX = Triangulo::GetGlobalFromHomogeneous(cPts7[ni], P1, P2, P3);

                res += cWeights7[ni] * func(i, j, ksi, posGlobalX);

            }

            const double A = ((P2 - P1).Cross(P3 - P1)).Mod() / 2.0;

            return res * 2.0 * A;
        }

        else
        {
            const Vetor Pp1 = ((P1 * 1.0) + P2) / 2.0;
            const Vetor Pp2 = ((P2 * 1.0) + P3) / 2.0;
            const Vetor Pp3 = ((P3 * 1.0) + P1) / 2.0;

            res += IntegrateWithSubHStar(P1, Pp1, Pp3, ksi,
                func, i, j, nivel - 1);

            res += IntegrateWithSubHStar(Pp1, Pp2, Pp3, ksi,
                func, i, j, nivel - 1);

            res += IntegrateWithSubHStar(Pp1, P2, Pp2, ksi,
                func, i, j, nivel - 1);

            res += IntegrateWithSubHStar(Pp2, P3, Pp3,  ksi,
                func, i, j, nivel - 1);

            return res;
        }

    }

private:
    const Matriz cPts1 = Matriz(1, 3, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    const Matriz cWeights1 = Matriz(1, 1, {1.0}) * 0.5;

      
    const Matriz cPts3 = Matriz(3, 3, { 1.0 / 2.0, 1.0 / 2.0, 0.0,
                            0, 1.0 / 2.0, 1.0 / 2.0,
                            1. / 2.0, 0.0, 1.0 / 2.0 });
    const Matriz cWeights3 = Matriz(3, 1, {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0}) * 0.5;

    
    const std::vector<Vetor> cPts7 = { { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0},
                                        {0.79742699, 0.10128651, 0.10128651},
                                        {0.10128651, 0.79742699, 0.10128651},
                                        {0.10128651, 0.10128651, 0.79742699},
                                        {0.05971587, 0.47014206, 0.47014206},
                                        {0.47014206, 0.05971587, 0.47014206},
                                        {0.47014206, 0.47014206, 0.05971587 } };

    const std::vector<double> cWeights7 = { 0.22500000 * 0.5,
    0.12593918*0.5,
    0.12593918*0.5,
    0.12593918*0.5,
    0.13239415*0.5,
    0.13239415*0.5,
    0.13239415*0.5 };

    const std::vector<Vetor> cPts13 = { 
                                   { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0},
                                   {0.479308067841920, 0.260345966079040, 0.260345966079040},
                                   {0.260345966079040, 0.479308067841920, 0.260345966079040},
                                   {0.260345966079040, 0.260345966079040, 0.479308067841920},
                                   {0.869739794195568, 0.065130102902216, 0.065130102902216},
                                   {0.065130102902216, 0.869739794195568, 0.065130102902216},
                                   {0.065130102902216, 0.065130102902216, 0.869739794195568},
                                   {0.638444188569810, 0.312865496004874, 0.048690315425316},
                                   {0.048690315425316, 0.638444188569810, 0.312865496004874},
                                   {0.312865496004874, 0.048690315425316, 0.638444188569810},
                                   {0.638444188569810, 0.048690315425316, 0.312865496004874},
                                   {0.312865496004874, 0.638444188569810, 0.048690315425316},
                                   {0.048690315425316, 0.312865496004874, 0.638444188569810} };

    const std::vector<double> cWeights13 = { -0.149570044467682*0.5,
                                            0.175915257433208  *0.5,
                                            0.175915257433208  *0.5,
                                            0.175915257433208  *0.5,
                                            0.053347235608838  *0.5,
                                            0.053347235608838  *0.5,
                                            0.053347235608838  *0.5,
                                            0.077113760890257  *0.5,
                                            0.077113760890257  *0.5,
                                            0.077113760890257  *0.5,
                                            0.077113760890257  *0.5,
                                            0.077113760890257  *0.5,
                                            0.077113760890257  *0.5  };
  
};