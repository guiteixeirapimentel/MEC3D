#define _USE_MATH_DEFINES
#include "GeometryCreator.h"
#include "Matriz.h"
#include "Quad.h"

void GeometryCreator::GetCylinderInternalPressure(double R, double L, double h, double press_int,
	double delta_theta, double delta_y, double delta_r,
	std::vector<Quad>& outQuads, std::vector<CondContorno>& outDelocs, std::vector<CondContorno>& outPress)
{
    const double Rint = R - h;
    
    const size_t Nh = h / delta_r;

    const size_t NL = (L / delta_y);
    const size_t Ntheta = (360 / delta_theta);

    const size_t n_quads = 2 * (NL * Ntheta) + 2*(Nh * Ntheta);

    std::vector<Quad> pontos_quad;
    pontos_quad.resize(n_quads); //= zeros(4 * n_quads, 3);

    std::vector<CondContorno> dp_quad;// = [];% zeros((Nh * Ntheta) / 2, 3);

    std::vector<CondContorno> fp_quad; // = [];% zeros(3 * (((L / delta_y) + (360 / delta_theta))) + (Nh * Ntheta) / 2, 3);

    size_t ipontos = 0;
    double y_offset = 0;
    size_t iquads = 0;
    size_t ifp = 0;
    size_t idp = 0;

    for (size_t iL = 0; iL < NL; iL++)
    {
        double theta = 0;
        for (size_t itheta = 0; itheta < Ntheta; itheta++)
        {
            Ponto x0 = { R * cos(M_PI / 180.0 * (theta)), y_offset, R * sin(M_PI / 180.0 * theta) };
            Ponto x1 = { R * cos(M_PI / 180.0 * (theta)), y_offset + delta_y, R * sin(M_PI / 180.0 * theta) };
            Ponto x2 = { R * cos(M_PI / 180.0 * (theta + delta_theta)), y_offset, R * sin(M_PI / 180.0 * (theta + delta_theta)) };
            Ponto x3 = { R * cos(M_PI / 180.0 * (theta + delta_theta)), y_offset + delta_y, R * sin(M_PI / 180.0 * (theta + delta_theta)) };

            pontos_quad[iquads].p1 = x0;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p2 = x1;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p3 = x2;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p4 = x3;
            ipontos = ipontos + 1;            

            iquads = iquads + 1;

            Ponto x0int = { Rint * cos(M_PI / 180.0 * theta), y_offset, Rint * sin(M_PI / 180.0 * theta) };
            Ponto x1int = { Rint * cos(M_PI / 180.0 * (theta + delta_theta)), y_offset, Rint * sin(M_PI / 180.0 * (theta + delta_theta)) };
            Ponto x2int = { Rint * cos(M_PI / 180.0 * theta), y_offset + delta_y, Rint * sin(M_PI / 180.0 * theta) };
            Ponto x3int = { Rint * cos(M_PI / 180.0 * (theta + delta_theta)), y_offset + delta_y, Rint * sin(M_PI / 180.0 * (theta + delta_theta)) };

            pontos_quad[iquads].p1 = x0int;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p2 = x1int;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p3 = x2int;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p4 = x3int;
            ipontos = ipontos + 1;


            Vetor normal_quad = { cos(M_PI / 180.0 * (theta + (delta_theta / 2))), 0, sin(M_PI / 180.0 * (theta + (delta_theta / 2))) };

            fp_quad.push_back({ (int)iquads, 1, normal_quad(1) * press_int });
            ifp = ifp + 1;
            fp_quad.push_back({ (int)iquads, 2, normal_quad(2) * press_int });
            ifp = ifp + 1;
            fp_quad.push_back({ (int)iquads, 3, normal_quad(3) * press_int });
            ifp = ifp + 1;

            theta = theta + delta_theta;

            iquads = iquads + 1;
        }    

        y_offset = y_offset + delta_y;
    }

    double theta = 0;
    for (size_t itheta = 0; itheta < Ntheta; itheta++)
    {
        double hoffset = 0;
        for (size_t ih = 0; ih < Nh; ih++)
        {
            double Rh = Rint + hoffset;

            Ponto x0h = { Rh * cos(M_PI / 180.0 * theta), 0, Rh * sin(M_PI / 180.0 * theta) };
            Ponto x1h = { Rh * cos(M_PI / 180.0 * (theta + delta_theta)), 0, Rh * sin(M_PI / 180.0 * (theta + delta_theta)) };
            Ponto x2h = { (Rh + delta_r) * cos(M_PI / 180.0 * theta), 0, (Rh + delta_r) * sin(M_PI / 180.0 * theta) };
            Ponto x3h = { (Rh + delta_r) * cos(M_PI / 180.0 * (theta + delta_theta)), 0,
                (Rh + delta_r) * sin(M_PI / 180.0 * (theta + delta_theta)) };

            pontos_quad[iquads].p1 = x0h;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p2 = x2h;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p3 = x1h;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p4 = x3h;
            ipontos = ipontos + 1;

            //% dp_quad(idp, :) = [iquads, 2, 0];
            //% idp = idp + 1;


            iquads = iquads + 1;

            x0h = { Rh * cos(M_PI / 180.0 * theta), L, Rh * sin(M_PI / 180.0 * theta) };
            x1h = { (Rh + delta_r) * cos(M_PI / 180.0 * theta), L, (Rh + delta_r) * sin(M_PI / 180.0 * theta) };
            x2h = { Rh * cos(M_PI / 180.0 * (theta + delta_theta)), L, Rh * sin(M_PI / 180.0 * (theta + delta_theta)) };
            x3h = { (Rh + delta_r) * cos(M_PI / 180.0 * (theta + delta_theta)), L, (Rh + delta_r) * sin(M_PI / 180.0 * (theta + delta_theta)) };

            pontos_quad[iquads].p1 = x0h;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p2 = x2h;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p3 = x1h;
            ipontos = ipontos + 1;
            pontos_quad[iquads].p4 = x3h;
            ipontos = ipontos + 1;

            //% fp_quad(ifp, :) = [iquads, 2, press_long];
            //% ifp = ifp + 1;
            //
            //% dp_quad(idp, :) = [iquads, 2, 0];
            //% idp = idp + 1;

            iquads = iquads + 1;

            hoffset = hoffset + delta_r;
        }
                
        theta = theta + delta_theta;          
    }

    outQuads = pontos_quad;
    outDelocs = dp_quad;
    outPress = fp_quad;
}

void GeometryCreator::GetCrucifix(std::vector<Quad>& outQuads, std::vector<CondContorno>& outDelocs,
    std::vector<CondContorno>& outPress)
{

    //% valores de força / área prescrita
    const double t2 = 1e6;
    const double t3 = 1e6;

    //% lados do quadrado e comprimento geometria
    const double t = 0.2;
    const double L = 0.4;

    Matriz R90X1(3, 3, { 1.0,  0.0,  0.0,
                         0.0,  0.0, -1.0,
                         0.0,  1.0,  0.0 });

    Quad bracoDirFrente = {
        //% frente
        {-0.5*t, 0.5*t + L/2, -0.5*t    },
        {-0.5*t, 0.5*t + L/2, 0.5*t     },
        {-0.5*t, 0.5*t + L, -0.5*t },
        {-0.5*t, 0.5*t + L, 0.5*t }
        };

    Quad bracoDirTras = {
        //% trás
        { 0.5*t, 0.5*t + L/2, -0.5*t     },
        { 0.5*t, 0.5*t + L, -0.5*t },
        { 0.5*t, 0.5*t + L/2, 0.5*t      },
        { 0.5*t, 0.5*t + L, 0.5*t  }
        };
        
    Quad topoBracoDir = {
        { -0.5 * t, 0.5 * t + L/2, -0.5 * t},
        { -0.5 * t, 0.5 * t + L, -0.5 * t},
        { 0.5 * t, 0.5 * t + L/2, -0.5 * t},
        { 0.5 * t, 0.5 * t + L, -0.5 * t}
        };

    Quad baixoBracoDir = {
        { -0.5*t, 0.5*t + L/2, 0.5*t      },
        { 0.5*t, 0.5*t + L/2, 0.5*t       },
        { -0.5*t, 0.5*t + L, 0.5*t  },
        { 0.5*t, 0.5*t + L, 0.5*t   }
        };

    Quad bracoDirFrenteH = {
        //% frente
        {-0.5 * t, 0.5 * t, -0.5 * t    },
        {-0.5 * t, 0.5 * t, 0.5 * t     },
        {-0.5 * t, 0.5 * t + L / 2, -0.5 * t},
        {-0.5 * t, 0.5 * t + L / 2, 0.5 * t }
    };

    Quad bracoDirTrasH = {
        //% trás
        { 0.5 * t, 0.5 * t, -0.5 * t     },
        { 0.5 * t, 0.5 * t + L / 2, -0.5 * t },
        { 0.5 * t, 0.5 * t, 0.5 * t      },
        { 0.5 * t, 0.5 * t + L / 2, 0.5 * t  }
    };

    Quad topoBracoDirH = {
        { -0.5 * t, 0.5 * t, -0.5 * t},
        { -0.5 * t, 0.5 * t + L / 2, -0.5 * t},
        { 0.5 * t, 0.5 * t, -0.5 * t},
        { 0.5 * t, 0.5 * t + L / 2, -0.5 * t}
    };

    Quad baixoBracoDirH = {
        { -0.5 * t, 0.5 * t, 0.5 * t      },
        { 0.5 * t, 0.5 * t, 0.5 * t       },
        { -0.5 * t, 0.5 * t + L / 2, 0.5 * t  },
        { 0.5 * t, 0.5 * t + L / 2, 0.5 * t   }
    };

    Quad fimBracoDir = {
        { -0.5*t, 0.5*t + L, -0.5*t },
        { -0.5*t, 0.5*t + L, 0.5*t  },
        { 0.5*t,  0.5*t  + L, -0.5*t  },
        { 0.5*t,  0.5*t  + L, 0.5*t   }
        };

    Quad tampaoCima = {
        { -0.5 * t, 0.5 * t, -0.5 * t },
        { -0.5 * t, 0.5 * t, 0.5 * t  },
        { 0.5 * t, 0.5 * t , -0.5 * t  },
        { 0.5 * t, 0.5 * t , 0.5 * t   }
    };

    //% Quad points
    std::vector<Quad> quad_points = 
    {
        //% faces centro
        //% frente quad 1
            {
                { -0.5 * t, -0.5 * t, -0.5 * t },
                { -0.5 * t, -0.5 * t, 0.5 * t },
                { -0.5 * t, 0.5 * t, -0.5 * t },
                { -0.5 * t, 0.5 * t, 0.5 * t}
            },

        //% trás quad 2
            {
                { 0.5 * t, -0.5 * t, -0.5 * t},
                { 0.5 * t, 0.5 * t, -0.5 * t },
                { 0.5 * t, -0.5 * t, 0.5 * t },
                { 0.5 * t, 0.5 * t, 0.5 * t  }
            },
        //% "braço" direito
        bracoDirFrente, //% quad 3
        bracoDirTras, //% quad 4
        fimBracoDir, //% quad 5
        topoBracoDir, //%quad 6
        baixoBracoDir, //% quad 7
        
        bracoDirFrenteH, //% quad 8
        bracoDirTrasH, //% quad 9
        topoBracoDirH, //%quad 10
        baixoBracoDirH, //% quad 11

        //% "braço" esquerdo
        (R90X1 * R90X1 * bracoDirFrente),//'; //% quad 12
        (R90X1 * R90X1 * bracoDirTras),//'; //% quad 13
        (R90X1 * R90X1 * fimBracoDir),//'; //% quad 14
        (R90X1 * R90X1 * topoBracoDir),//'; //% quad 15
        (R90X1 * R90X1 * baixoBracoDir),//'; //% quad 16
        
        (R90X1* R90X1* bracoDirFrenteH),//'; //% quad 17
        (R90X1* R90X1* bracoDirTrasH),//'; //% quad 18
        (R90X1* R90X1* topoBracoDirH),//'; //% quad 19
        (R90X1* R90X1* baixoBracoDirH),//'; //% quad 20

        //% "braço" cima
        (R90X1 * bracoDirFrente),//'; % quad 21 3
        (R90X1 * bracoDirTras),//'; % quad 22 4 
        (R90X1 * fimBracoDir),//'; % quad 23 5 
        (R90X1 * topoBracoDir),//'; % quad 24 6 
        (R90X1 * baixoBracoDir),//'; % quad 25 7

        (R90X1* bracoDirFrenteH),//'; % quad 26 8
        (R90X1* bracoDirTrasH),//'; % quad 27 9
        (R90X1* topoBracoDirH),//'; % quad 28 10
        (R90X1* baixoBracoDirH),//'; % quad 29 11

        //% "braço" baixo
        (R90X1 * R90X1 * R90X1 * bracoDirFrente),//')'; % quad 30 12
        (R90X1 * R90X1 * R90X1 * bracoDirTras),//')'; % quad 31 13 
        (R90X1 * R90X1 * R90X1 * fimBracoDir),//')'; % quad 32 14 
        (R90X1 * R90X1 * R90X1 * topoBracoDir),//')'; % quad 33 15
        (R90X1 * R90X1 * R90X1 * baixoBracoDir),//')'; % quad 34 16

        (R90X1* R90X1* R90X1* bracoDirFrenteH),//')'; % quad 35 17
        (R90X1* R90X1* R90X1* bracoDirTrasH),//')'; % quad 36 18
        (R90X1* R90X1* R90X1* topoBracoDirH),//')'; % quad 37 19
        (R90X1* R90X1* R90X1* baixoBracoDirH)//,//')'; % quad 38 20

        //(R90X1* R90X1* tampaoCima), // 21
        //tampaoCima // 22
    };
//clear R90X1 bracoDirFrente bracoDirTras fimBracoDir topoBracoDir baixoBracoDir;

//% id quad, grau de liberdade(x1, x2 ou x3) e valor prescrito
    std::vector<CondContorno> quad_dp = {
           //{5, 1, 0},//;% fim direito
           //{5, 2, 0},//;% fim direito
           //{5, 3, 0},//;% fim direito
           //{23, 1, 0},//;% fim cima
           //{23, 2, 0},//;% fim cima
           //{23, 3, 0}//;% fim cima
    };

//% id quad, grau de liberdade(x1, x2 ou x3) e valor prescrito
    std::vector<CondContorno> quad_fp = {
        { 5, 2, t2},    //; % fim direito
        { 14, 2, -t2 }, //;% fim esquerdo
        //{ 23, 3, t3 }, // ; % fim cima
        //{ 32, 3, -t3 },   //;% fim baixo

        {23, 3, t3},
        {32, 3, -t3}
    };


    outQuads = quad_points;

    outDelocs = quad_dp;

    outPress = quad_fp;
}

void GeometryCreator::GetAxialStress(std::vector<Quad>& outQuads, std::vector<CondContorno>& outDelocs,
    std::vector<CondContorno>& outPress)
{
    const double b = 1.0;
    const double a = 1.0;
    const double h = 20.0;

    const double deltah = 1.0;

    // nomes baseados quando cilindro olhado de cima.

    Quad base =
    {
        {b, a, 0.0},
        {b, 0.0, 0.0},
        {0.0, a, 0.0},
        {0.0, 0.0, 0.0}
    };

    Quad topo =
    {
        {0.0, 0.0, h},
        {b, 0.0, h},
        {0.0, a, h},
        {b, a, h}
    };

    Quad lateralBaixo =
    {
        {0.0, 0.0, 0.0},
        {b, 0.0, 0.0},
        {0.0, 0.0, h},
        {b, 0.0, h}
    };

    Quad lateralCima =
    {
        {b, a, h},
        {b, a, 0.0},
        {0.0, a, h},
        {0.0, a, 0.0}
    };

    Quad lateralDireita =
    {
        {b, 0.0, 0.0},
        {b, a, 0.0},
        {b, 0.0, h},
        {b, a, h}
    };

    Quad lateralEsquerda =
    {
        {0.0, a, h},
        {0.0, a, 0.0},
        {0.0, 0.0, h},
        {0.0, 0.0, 0.0}
    };

    outQuads.push_back(base);
    outQuads.push_back(topo);
    outQuads.push_back(lateralDireita);
    outQuads.push_back(lateralEsquerda);
    outQuads.push_back(lateralBaixo);
    outQuads.push_back(lateralCima);

    //outPress.push_back({ 1, 1, -1.0 });
    //outPress.push_back({ 1, 2, -1.0 });
    //outPress.push_back({ 1, 3, -1.0 });

    outDelocs.push_back({ 1, 1, 0.0 });
    outDelocs.push_back({ 1, 2, 0.0 });
    outDelocs.push_back({ 1, 3, 0.0 });

    //outPress.push_back({ 2, 1, +1.0 });
    //outPress.push_back({ 2, 2, +1.0 });
    outPress.push_back({ 2, 3, -1.0 });
}

void GeometryCreator::GetBendedRectBar(std::vector<Quad>& outQuads, std::vector<CondContorno>& outDelocs,
    std::vector<CondContorno>& outPress)
{
    const double b = 1.0;
    const double a = 1.0;
    const double h = 3.0;

    const double deltah = 1.0;

    // nomes baseados quando cilindro olhado de cima.

    Quad base =
    {
        {b, a, 0.0},
        {b, 0.0, 0.0},
        {0.0, a, 0.0},
        {0.0, 0.0, 0.0}
    };

    Quad topo =
    {
        {0.0, 0.0, h},
        {b, 0.0, h},
        {0.0, a, h},
        {b, a, h}
    };

    /*Quad lateralBaixo1 =
    {
        {0.0, 0.0, 0.0},
        {b, 0.0, 0.0},
        {0.0, 0.0, h/3},
        {b, 0.0, h/3}
    };

    Quad lateralCima1 =
    {
        {b, a, h/3},
        {b, a, 0.0},
        {0.0, a, h/3},
        {0.0, a, 0.0}
    };

    Quad lateralDireita1 =
    {
        {b, 0.0, 0.0},
        {b, a, 0.0},
        {b, 0.0, h/3},
        {b, a, h/3}
    };

    Quad lateralEsquerda1 =
    {
        {0.0, a, h/3},
        {0.0, a, 0.0},
        {0.0, 0.0, h/3},
        {0.0, 0.0, 0.0}
    };

    Quad lateralBaixo2 =
    {
        {0.0, 0.0, h / 3},
        {b, 0.0, h / 3},
        {0.0, 0.0, h*2.0/3.0},
        {b, 0.0, h*2.0/3.0}
    };

    Quad lateralCima2 =
    {
        {b, a, h*2.0/3.0},
        {b, a, h / 3},
        {0.0, a, h*2.0/3.0},
        {0.0, a, h / 3}
    };

    Quad lateralDireita2 =
    {
        {b, 0.0, h / 3},
        {b, a, h / 3},
        {b, 0.0, h*2.0/3.0},
        {b, a, h*2.0/3.0}
    };

    Quad lateralEsquerda2 =
    {
        {0.0, a, h*2.0/3.0},
        {0.0, a, h / 3},
        {0.0, 0.0, h*2.0/3.0},
        {0.0, 0.0, h / 3}
    };

    Quad lateralBaixo3 =
    {
        {0.0, 0.0, h * 2.0 / 3.0},
        {b, 0.0, h * 2.0 / 3.0},
        {0.0, 0.0, h},
        {b, 0.0, h}
    };

    Quad lateralCima3 =
    {
        {b, a, h},
        {b, a, h * 2.0 / 3.0},
        {0.0, a, h},
        {0.0, a, h * 2.0 / 3.0}
    };

    Quad lateralDireita3 =
    {
        {b, 0.0, h * 2.0 / 3.0},
        {b, a, h * 2.0 / 3.0},
        {b, 0.0, h},
        {b, a, h}
    };

    Quad lateralEsquerda3 =
    {
        {0.0, a, h},
        {0.0, a, h * 2.0 / 3.0},
        {0.0, 0.0, h},
        {0.0, 0.0, h * 2.0 / 3.0}
    };*/

    outQuads.push_back(base);
    outQuads.push_back(topo);

    for (size_t i = 0; i < size_t(h); i++)
    {
        const Quad lateralCima =
        {
            {b, a, h * (i+1) / h},
            {b, a, h * i / h},
            {0.0, a, h * (i + 1) / h},
            {0.0, a, h * i / h}
        };

        outQuads.push_back(lateralCima);

        outPress.push_back({ int(i + 3), 1, 1.0 });
    }

    for (size_t i = 0; i < size_t(h); i++)
    {
        Quad lateralBaixo =
        {
            {0.0, 0.0, h * i / h},
            {b, 0.0, h * i / h},
            {0.0, 0.0, h * (i + 1) / h},
            {b, 0.0, h * (i + 1) / h}
        };

        Quad lateralDireita =
        {
            {b, 0.0, h * i / h},
            {b, a, h * i / h},
            {b, 0.0, h * (i + 1) / h},
            {b, a, h * (i + 1) / h}
        };

        Quad lateralEsquerda =
        {
            {0.0, a, h * (i + 1) / h},
            {0.0, a, h * i / h},
            {0.0, 0.0, h * (i + 1) / h},
            {0.0, 0.0, h * i / h}
        };

        outQuads.push_back(lateralBaixo);
        outQuads.push_back(lateralDireita);
        outQuads.push_back(lateralEsquerda);
    }

    //outQuads.push_back(lateralCima1);
    //outQuads.push_back(lateralCima2);
    //outQuads.push_back(lateralCima3);
    //
    //outQuads.push_back(lateralDireita1);
    //outQuads.push_back(lateralEsquerda1);
    //outQuads.push_back(lateralBaixo1);
    //
    //outQuads.push_back(lateralDireita2);
    //outQuads.push_back(lateralEsquerda2);
    //outQuads.push_back(lateralBaixo2);
    //
    //outQuads.push_back(lateralDireita3);
    //outQuads.push_back(lateralEsquerda3);
    //outQuads.push_back(lateralBaixo3);

    //outPress.push_back({ 1, 1, -1.0 });
    //outPress.push_back({ 1, 2, -1.0 });
    //outPress.push_back({ 1, 3, -1.0 });

    outDelocs.push_back({ 1, 1, 0.0 });
    //outDelocs.push_back({ 1, 2, 0.0 });
    //outDelocs.push_back({ 1, 3, 0.0 });
    
    outDelocs.push_back({ 2, 1, 0.0 });

    //outPress.push_back({ 2, 1, +1.0 });
    //outPress.push_back({ 2, 2, +1.0 });
    //outPress.push_back({ 2, 3, -1.0 });

    //outPress.push_back({ 3, 1, -1.0 });
    //outPress.push_back({ 4, 1, -1.0 });
    //outPress.push_back({ 5, 1, -1.0 });
}

void GeometryCreator::GetSwimmingPool(std::vector<Quad>& outQuads, std::vector<CondContorno>& outDelocs,
	std::vector<CondContorno>& outPress)
{
	outQuads.clear();
	outDelocs.clear();
	outPress.clear();

	Quad S1 = {
		{3.0, -1.0, 0.0 },
		{3.0, 1.0, 0.0 },
		{5.0, -1.0, 0.0},
		{5.0, 1.0, 0.0}
	};

	Quad S2 = {
		{-3.0, -3.0, 3.0},
		{0.0, -3.0, 3.0},
		{-3.0, 3.0, 3.0},
		{0.0, 3.0, 3.0}
	};

	Quad S3 = {
		{0.0, -3.0, 3.0},	
		{0.0, -3.0, 0.0},
		{0.0, 3.0, 3.0},
		{0.0, 3.0, 0.0}
	};

	Quad S4 = {
		{-3.0, -3.0, 3.0},
		{-3.0, -3.0, 0.0},
		{0.0, -3.0, 3.0},
		{0.0, -3.0, 0.0}
	};

	Quad S5 = {
		{-3.0, 3.0, 3.0},
		{-3.0, 3.0, 0.0},
		{-3.0, -3.0, 3.0},
		{-3.0, -3.0, 0.0}
	};

	Quad S6 = {
		{0.0, 3.0, 3.0},
		{0.0, 3.0, 0.0},
		{-3.0, 3.0, 3.0},
		{-3.0, 3.0, 0.0}
	};

	outQuads.push_back(S1);
	outQuads.push_back(S2);
	outQuads.push_back(S3);
	outQuads.push_back(S4);
	outQuads.push_back(S5);
	outQuads.push_back(S6);

	outPress.push_back({ 1, 3, 150e3 });
}