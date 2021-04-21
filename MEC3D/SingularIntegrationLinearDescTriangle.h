#pragma once
#include <cmath>
#include "Matriz.h"
#include "Triangulo.h"
#include "UStarFunNoSing.h"
#include "PStarFunNoSing.h"
#include "MathUtils.h"

class SingularIntegrationLinearDescTriangle
{
public:
    SingularIntegrationLinearDescTriangle() {};
    ~SingularIntegrationLinearDescTriangle() {};

    double IntegrateUStar(const Triangulo& triangulo, const Vetor& ksi,
        const UStarFunNoSing& func, int i, int j, int indiceKsi, double maxRes = 1e-10) const
    {
        double t = IntegrateUStar(triangulo, ksi, func, i, j, indiceKsi, 1);

        double res = 0.0;
        int index;

        for (index = 2; index < 4; index++)
        {
            const double t2 = IntegrateUStar(triangulo, ksi, func, i, j, indiceKsi, index);

            res = std::fabs(t2 - t);

            t = t2;

            if (res < maxRes)
            {
                break;
            }
        }

        //std::cout << res << " - "<<  index <<std::endl;
        return t;
    }

    double IntegratePStar(const Triangulo& triangulo, const Vetor& ksi,
        const PStarFunNoSing& func, int i, int j, int indiceKsi, double maxRes = 1e-10) const
    {
        double t = IntegratePStar(triangulo, ksi, func, i, j, indiceKsi, 1);

        double res = 0.0;
        int index;
        int indexm1 = 1;

        for (index = 2; index < 6; index += indexm1)
        {
            indexm1 = index - indexm1;
            //std::cout << index << std::endl;

            const double t2 = IntegratePStar(triangulo, ksi, func, i, j, indiceKsi, index);

            res = std::fabs(t2 - t);

            t = t2;

            if (res < maxRes)
            {
                break;
            }
        }

        //std::cout << res << " - "<<  index <<std::endl;
        return t;
    }

    Vetor CoordLocal(const Vetor& no1, const Vetor& no2, const Vetor& no3, const Vetor& pontoFonte, const Vetor& pontoCentral) const
    {

        /* function f019 = func019(no1, no2, no3, pF, pC)

        % Livro de GAAL - pág. 362.
             % Devolve a coordenada do ponto fonte pF a partir da coordenada local
             % com origem no ponto campo pC.
             %
             % Direção U1 :
         V1 = [no1 - pF];
         U1 = V1 / norm(V1);
         % Direção U3 :
         V2 = [no2 - pF];
         V3 = cross(V1, V2);
         U3 = V3 / norm(V3);
         % Direção U2 :
         U2 = cross(U3, U1);
         f019 = inv([U1, U2, U3]) * (pC - pF);*/

        Vetor V1 = no1 - pontoFonte;
        Vetor U1 = V1 / V1.Mod();

        Vetor V2 = no2 - pontoFonte;

        Vetor V3 = V1.Cross(V2);

        Vetor U3 = V3 / V3.Mod();

        Vetor U2 = U3.Cross(U1);

        Matriz M(3, 3, { U1.cX1, U2.cX1, U3.cX1,
                        U1.cX2, U2.cX2, U3.cX2,
                        U1.cX3, U2.cX3, U3.cX3, });

        return CalcInvMatriz(M) * (pontoCentral - pontoFonte);
    }

    Matriz Calcabc(const Vetor& no1, const Vetor& no2, const Vetor& no3) const
    {
        /* %% Identifica as constantes ai, bi e ci do elemento.
             function f020 = func020(no1, no2, no3)
             % Equação do tipo ei = ai.Y1 + bi.Y2 + ci(ver Calderón 4.69).
             %
             %
             %
             L1 = [no1(1:2, 1); 1]';
             L2 = [no2(1:2, 1); 1]';
             L3 = [no3(1:2, 1); 1]';
             B = [L1; L2; L3];
         % Determinação do ponto e1 :
         A = [1; 0; 0];
         F1 = (inv(B) * A);
         % Determinação do ponto e2 :
         A = [0; 1; 0];
         F2 = (inv(B) * A);
         % Determinação do ponto e3 :
         A = [0; 0; 1];
         F3 = (inv(B) * A);

         f020 = [F1, F2, F3];*/

        const Matriz B(3, 3, { no1.cX1, no1.cX2, 1.0,
                        no2.cX1, no2.cX2, 1.0,
                        no3.cX1, no3.cX2, 1.0 });

        const Matriz Binv = CalcInvMatriz(B);

        const Vetor F1 = Binv * Vetor(1.0, 0.0, 0.0);
        const Vetor F2 = Binv * Vetor(0.0, 1.0, 0.0);
        const Vetor F3 = Binv * Vetor(0.0, 0.0, 1.0);

        return Matriz(3, 3, { F1.cX1, F2.cX1, F3.cX1,
                            F1.cX2, F2.cX2, F3.cX2,
                            F1.cX3, F2.cX3, F3.cX3 });
    }

private:
    double IntegrateUStar(const Triangulo& triangulo, const Vetor& ksi,
        const UStarFunNoSing& func, int i, int j, int indiceKsi, int subDivs) const
    {
        std::vector<double> resv;
        resv.resize(subDivs, 0.0);

        const Vetor p1 = triangulo.cP1 - triangulo.cCM;
        const Vetor p2 = triangulo.cP2 - triangulo.cCM;
        const Vetor p3 = triangulo.cP3 - triangulo.cCM;

        const Vetor ponto1Y = CoordLocal(triangulo.cP1, triangulo.cP2, triangulo.cP3, triangulo.cCM, triangulo.cP1);
        const Vetor ponto2Y = CoordLocal(triangulo.cP1, triangulo.cP2, triangulo.cP3, triangulo.cCM, triangulo.cP2);
        const Vetor ponto3Y = CoordLocal(triangulo.cP1, triangulo.cP2, triangulo.cP3, triangulo.cCM, triangulo.cP3);

        // verifica se a rotação eliminou a componente Z...
        assert(ponto1Y.cX3 < 1.0e-6 && ponto2Y.cX3 < 1.0e-6 && ponto3Y.cX3 < 1.0e-6);

        // matriz [ai bi ci]
        //const Matriz abc1 = Calcabc(ponto1Y, ponto2Y, ponto3Y);
        const Matriz abc(3, 3,
            {
            (ponto2Y.cX2 - ponto3Y.cX2) / (2.0 * triangulo.cArea),
            (ponto3Y.cX1 - ponto2Y.cX1) / (2.0 * triangulo.cArea),
            (ponto2Y.cX1 * ponto3Y.cX2 - ponto3Y.cX1 * ponto2Y.cX2) / (2.0 * triangulo.cArea),

            (ponto3Y.cX2 - ponto1Y.cX2) / (2.0 * triangulo.cArea),
            (ponto1Y.cX1 - ponto3Y.cX1) / (2.0 * triangulo.cArea),
            (ponto3Y.cX1 * ponto1Y.cX2 - ponto1Y.cX1 * ponto3Y.cX2) / (2.0 * triangulo.cArea),

            (ponto1Y.cX2 - ponto2Y.cX2) / (2.0 * triangulo.cArea),
            (ponto2Y.cX1 - ponto1Y.cX1) / (2.0 * triangulo.cArea),
            (ponto1Y.cX1 * ponto2Y.cX2 - ponto2Y.cX1 * ponto1Y.cX2) / (2.0 * triangulo.cArea)
            });

        const double ai = abc(indiceKsi-1, 0);
        const double bi = abc(indiceKsi-1, 1);
        const double ci = abc(indiceKsi-1, 2);


        const double dTheta = 2.0 * M_PI / double(subDivs);

        for (int index = 0; index < subDivs; index++)
        {
            const double a = index * dTheta;
            const double b = (index + 1) * dTheta;

            for (int ni = 0; ni < cPts11.size(); ni++)
            {
                const double thetai = (b - a) / 2.0 * cPts11[ni] + (a + b) / 2.0;

                double rk = 0.0;


                if (thetai < triangulo.cTheta1)
                {
                    rk = -abc(2, 2) / (abc(2, 0) * cos(thetai) + abc(2, 1) * sin(thetai));
                }
                else if (thetai < triangulo.cTheta2)
                {
                    rk = -abc(0, 2) / (abc(0, 0) * cos(thetai) + abc(0, 1) * sin(thetai));
                }
                else
                {
                    rk = -abc(1, 2) / (abc(1, 0) * cos(thetai) + abc(1, 1) * sin(thetai));
                }

                const Vetor posEmCoordLocais = Vetor(cos(thetai), sin(thetai), 0.0) * rk;

                const double ksi1 = abc(0, 0) * posEmCoordLocais.cX1 + abc(0, 1) * posEmCoordLocais.cX2 + abc(0, 2);
                const double ksi2 = abc(1, 0) * posEmCoordLocais.cX1 + abc(1, 1) * posEmCoordLocais.cX2 + abc(1, 2);
                const double ksi3 = abc(2, 0) * posEmCoordLocais.cX1 + abc(2, 1) * posEmCoordLocais.cX2 + abc(2, 2);

                const Vetor posEmCoordGlobais = triangulo.GetGlobalFromHomogeneous({ ksi1, ksi2, ksi3 });

                const double Kg = func(i, j, ksi, posEmCoordGlobais);

                resv[index] += cWeights11[ni] * Kg * (ai * rk * rk / 2.0 * cos(thetai) + bi * rk * rk / 2.0 * sin(thetai) + ci * rk);
            }

            resv[index] *= (b - a) / 2;
        }

        double res = 0.0;

        for (int index = 0; index < resv.size(); index++)
        {
            res += resv[index];
        }

        return res;
    }

    double IntegratePStar(const Triangulo& triangulo, const Vetor& ksi,
        const PStarFunNoSing& func, int i, int j, int indiceKsi, int subDivs) const
    {
        std::vector<double> resv;
        resv.resize(subDivs, 0.0);

        const Vetor p1 = triangulo.cP1 - triangulo.cCM;
        const Vetor p2 = triangulo.cP2 - triangulo.cCM;
        const Vetor p3 = triangulo.cP3 - triangulo.cCM;

        const Vetor ponto1Y = CoordLocal(triangulo.cP1, triangulo.cP2, triangulo.cP3, triangulo.cCM, triangulo.cP1);
        const Vetor ponto2Y = CoordLocal(triangulo.cP1, triangulo.cP2, triangulo.cP3, triangulo.cCM, triangulo.cP2);
        const Vetor ponto3Y = CoordLocal(triangulo.cP1, triangulo.cP2, triangulo.cP3, triangulo.cCM, triangulo.cP3);

        // verifica se a rotação eliminou a componente Z...
        assert(ponto1Y.cX3 < 1.0e-6 && ponto2Y.cX3 < 1.0e-6 && ponto3Y.cX3 < 1.0e-6);

        // matriz [ai bi ci]
        //const Matriz abc1 = Calcabc(ponto1Y, ponto2Y, ponto3Y);
        const Matriz abc(3, 3,
            {
            (ponto2Y.cX2 - ponto3Y.cX2) / (2.0 * triangulo.cArea),
            (ponto3Y.cX1 - ponto2Y.cX1) / (2.0 * triangulo.cArea),
            (ponto2Y.cX1 * ponto3Y.cX2 - ponto3Y.cX1 * ponto2Y.cX2) / (2.0 * triangulo.cArea),

            (ponto3Y.cX2 - ponto1Y.cX2) / (2.0 * triangulo.cArea),
            (ponto1Y.cX1 - ponto3Y.cX1) / (2.0 * triangulo.cArea),
            (ponto3Y.cX1 * ponto1Y.cX2 - ponto1Y.cX1 * ponto3Y.cX2) / (2.0 * triangulo.cArea),

            (ponto1Y.cX2 - ponto2Y.cX2) / (2.0 * triangulo.cArea),
            (ponto2Y.cX1 - ponto1Y.cX1) / (2.0 * triangulo.cArea),
            (ponto1Y.cX1 * ponto2Y.cX2 - ponto2Y.cX1 * ponto1Y.cX2) / (2.0 * triangulo.cArea)
            });

        const double ai = abc(indiceKsi - 1, 0);
        const double bi = abc(indiceKsi - 1, 1);
        const double ci = abc(indiceKsi - 1, 2);


        const double dTheta = 2.0 * M_PI / double(subDivs);

        for (int index = 0; index < subDivs; index++)
        {
            const double a = index * dTheta;
            const double b = (index + 1) * dTheta;

            for (int ni = 0; ni < cPts11.size(); ni++)
            {
                const double thetai = (b - a) / 2.0 * cPts11[ni] + (a + b) / 2.0;

                double rk = 0.0;

                if (thetai < triangulo.cTheta1)
                {
                    rk = -abc(2, 2) / (abc(2, 0) * cos(thetai) + abc(2, 1) * sin(thetai));
                }
                else if (thetai < triangulo.cTheta2)
                {
                    rk = -abc(0, 2) / (abc(0, 0) * cos(thetai) + abc(0, 1) * sin(thetai));
                }
                else
                {
                    rk = -abc(1, 2) / (abc(1, 0) * cos(thetai) + abc(1, 1) * sin(thetai));
                }


                const Vetor posEmCoordLocais = Vetor(cos(thetai), sin(thetai), 0.0) * rk;

                const double ksi1 = abc(0, 0) * posEmCoordLocais.cX1 + abc(0, 1) * posEmCoordLocais.cX2 + abc(0, 2);
                const double ksi2 = abc(1, 0) * posEmCoordLocais.cX1 + abc(1, 1) * posEmCoordLocais.cX2 + abc(1, 2);
                const double ksi3 = abc(2, 0) * posEmCoordLocais.cX1 + abc(2, 1) * posEmCoordLocais.cX2 + abc(2, 2);

                const Vetor posEmCoordGlobais = triangulo.GetGlobalFromHomogeneous({ ksi1, ksi2, ksi3 });

                const double Kg = func(i, j, ksi, posEmCoordGlobais, triangulo.cNormalU);

                resv[index] += cWeights11[ni] * Kg * (ai * rk * cos(thetai) + bi * rk * sin(thetai) + ci * log(rk));
            }

            resv[index] *= (b - a) / 2;
        }

        double res = 0.0;

        for (int index = 0; index < resv.size(); index++)
        {
            res += resv[index];
        }

        return res;
    }

private:

    /*std::vector<double> cPts7 =
    {
     -0.960289856497536,
     -0.796666477413627,
     -0.525532409916329,
     -0.183434642495650,
     +0.183434642495650,
     +0.525532409916329,
     +0.796666477413627,
     +0.960289856497536
    };

    std::vector<double> cWeights7 =
    {
        0.101228536290376,
        0.222381034453374,
        0.313706645877887,
        0.362683783378362,
        0.362683783378362,
        0.313706645877887,
        0.222381034453374,
        0.101228536290376
    };*/

    std::vector<double> cPts11 =
    { 0.125233408511469, -0.125233408511469, 0.367831498998180, -0.367831498998180,
    0.587317954286617, -0.587317954286617, 0.769902674194305, -0.769902674194305,
    0.904117256370475, -0.904117256370475, 0.981560634246719, -0.981560634246719
    };
    std::vector<double> cWeights11 =
    { 0.249147045813403, 0.249147045813403, 0.233492536538355, 0.233492536538355,
    0.203167426723066, 0.203167426723066, 0.160078328543346, 0.160078328543346,
    0.106939325995318, 0.106939325995318, 0.047175336386512, 0.047175336386512 };

    std::vector<double> cPts48 =
    {
        0.998771007252426118601, -0.998771007252426118601,
        0.993530172266350757548, -0.993530172266350757548,
        0.984124583722826857745, -0.984124583722826857745,
        0.970591592546247250461, -0.970591592546247250461,
        0.952987703160430860723, -0.952987703160430860723,
        0.931386690706554333114, -0.931386690706554333114,
        0.905879136715569672822, -0.905879136715569672822,
        0.876572020274247885906, -0.876572020274247885906,
        0.843588261624393530711, -0.843588261624393530711,
        0.807066204029442627083, -0.807066204029442627083,
        0.767159032515740339254, -0.767159032515740339254,
        0.724034130923814654674, -0.724034130923814654674,
        0.677872379632663905212, -0.677872379632663905212,
        0.628867396776513623995, -0.628867396776513623995,
        0.577224726083972703818, -0.577224726083972703818,
        0.523160974722233033678, -0.523160974722233033678,
        0.466902904750958404545, -0.466902904750958404545,
        0.408686481990716729916, -0.408686481990716729916,
        0.348755886292160738160, -0.348755886292160738160,
        0.287362487355455576736, -0.287362487355455576736,
        0.224763790394689061225, -0.224763790394689061225,
        0.161222356068891718056, -0.161222356068891718056,
        0.097004699209462698930, -0.097004699209462698930,
        0.032380170962869362033, -0.032380170962869362033
    };

    std::vector<double> cWeights48 =
    {
        0.003153346052305838633, 0.003153346052305838633,
        0.007327553901276262102, 0.007327553901276262102,
        0.011477234579234539490, 0.011477234579234539490,
        0.015579315722943848728, 0.015579315722943848728,
        0.019616160457355527814, 0.019616160457355527814,
        0.023570760839324379141, 0.023570760839324379141,
        0.027426509708356948200, 0.027426509708356948200,
        0.031167227832798088902, 0.031167227832798088902,
        0.034777222564770438893, 0.034777222564770438893,
        0.038241351065830706317, 0.038241351065830706317,
        0.041545082943464749214, 0.041545082943464749214,
        0.044674560856694280419, 0.044674560856694280419,
        0.047616658492490474826, 0.047616658492490474826,
        0.050359035553854474958, 0.050359035553854474958,
        0.052890189485193667096, 0.052890189485193667096,
        0.055199503699984162868, 0.055199503699984162868,
        0.057277292100403215705, 0.057277292100403215705,
        0.059114839698395635746, 0.059114839698395635746,
        0.060704439165893880053, 0.060704439165893880053,
        0.062039423159892663904, 0.062039423159892663904,
        0.063114192286254025657, 0.063114192286254025657,
        0.063924238584648186624, 0.063924238584648186624,
        0.064466164435950082207, 0.064466164435950082207,
        0.064737696812683922503, 0.064737696812683922503
    };

    std::vector<double> cPts7 =
    {
     -0.949107912342759,
     -0.741531185599394,
     -0.405845151377397,
     0.0              ,
     +0.949107912342759,
     +0.741531185599394,
     +0.405845151377397
    };

    std::vector<double> cWeights7 =
    {
        0.129484966168870,
        0.279705391489277,
        0.381830050505119,
        0.417959183673469,
        0.381830050505119,
        0.279705391489277,
        0.129484966168870
    };
};