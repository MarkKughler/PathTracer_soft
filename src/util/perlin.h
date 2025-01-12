#pragma once

class cPerlin
{
public:
    cPerlin()
    {
        for (int i = 0; i < point_count; i++)
            rnd_vec[i] = unit_vector(cVec3::Random(-1.0, 1.0));
        
        GeneratePermutation(perm_x);
        GeneratePermutation(perm_y);
        GeneratePermutation(perm_z);
    }

    double Noise(const cVec3& p) const
    {
        auto u = p.x - std::floor(p.x);
        auto v = p.y - std::floor(p.y);
        auto w = p.z - std::floor(p.z);
        // Hermitian Smoothing
        //u = u * u * (3 - 2 * u);
        //v = v * v * (3 - 2 * v);
        //w = w * w * (3 - 2 * w);

        int i = int(std::floor(p.x));
        int j = int(std::floor(p.y));
        int k = int(std::floor(p.z));

        cVec3 c[2][2][2];
        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = rnd_vec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
        
        return VectorInterpolate(c, u, v, w);
    }

    double Turbulence(const cVec3& p, int depth) const
    {
        double accum = 0.0;
        cVec3 tmp_p = p;
        double weight = 1.0;

        for (int i = 0; i < depth; i++)
        {
            accum += weight * Noise(tmp_p);
            weight *= 0.5;
            tmp_p *= 2;
        }
        return std::fabs(accum);
    }


private:
    static const int point_count = 256;
    cVec3 rnd_vec[point_count];
    int perm_x[point_count];
    int perm_y[point_count];
    int perm_z[point_count];

    static void GeneratePermutation(int* p)
    {
        for (int i = 0; i < point_count; i++) p[i] = i;

        for (int ii = point_count - 1; ii > 0; ii--)
        {
            int target = Rnd(0, ii);
            std::swap<int>(p[ii], p[target]);
        }
    }


    static double TrilinearInterpolate(double c[2][2][2], double u, double v, double w)
    {
        double accum = 0.0;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                    accum += (i * u + (1.0 - i) * (1.0 - u)) * (j * v + (1.0 - j) * (1.0 - v)) * (k * w + (1.0 - k) * (1.0 - w)) * c[i][j][k];

        return accum;
    }

    static double VectorInterpolate(const cVec3 c[2][2][2], double u, double v, double w)
    {
        double uu = u * u * (3 - 2 * u);
        double vv = v * v * (3 - 2 * v);
        double ww = w * w * (3 - 2 * w);
        double accum = 0.0;
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                {
                    cVec3 weight(u - i, v - j, w - k);
                    accum += (i*uu + (1-i)*(1-uu)) * (j*vv + (1-j)*(1-vv)) * (k*ww + (1-k)*(1-ww)) * dot(c[i][j][k], weight);
                }
        return accum;
    }
};
