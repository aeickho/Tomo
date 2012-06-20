
namespace tomo
{
/*
  struct TemporaryN3Node
  {
    Vec3f normal[N][N][N];
    TriangleList triangles[N][N][N];
    BoundingBox box;

    void insert( Triangle* tri );

    RayPacket generateRay(unsigned k, unsigned u, unsigned v);
  };

#define ALL 0x15
  void TemporaryN3Node::insert( Triangle* tri )
  {
      unsigned k = tri->k; ku = tri->ku(); kv = tri->kv();
      float       org_k = tri->n_d - box.min[k];
      float       invdir_k = N / (box.max[k] - box.min[k]);

      for (unsigned u = 0; u < N; u++)
        for (unsigned v = 0; v < N; v++)
        {
          FloatPacket org_u(0,0,0,0), 
                      org_v(0,0,0,0);
          FloatPacket t = (org_k - tri->n_u * org_u - tri->n_v * org_v) * invdir_k;

          unsigned cmpTnear = (t >= 0), cmpTfar = (t <= N);
          if (!cmpTnear || !cmpTfar) return;

          FloatPacket hu = org_u - tri->au;
          FloatPacket hv = org_u - tri->av;
          FloatPacket bu = hv * tri->b_nu + hu * tri->b_nv;
          FloatPacket bv = hu * tri->c_nu + hv * tri->c_nv;

          unsigned cmpU = (bu >= 0), cmpV = (bv >= 0), cmpUV = (bu + bv <= 1.0);
          if (!cmpU || !cmpV || !cmpUV) return;

          float tmin = t.min(), float tmax = t.max();
          unsigned kmin = (unsigned)range(tmin,0,N-1);
          unsigned kmax = (unsigned)range(tmax,0,N-1);

          for (unsigned ki = kmin; ki <= kmax; ki++)
            triangles[ki][u][v].push_back(tri);
        }
  }


  void Voxels::build(const Triangles& triangles, BoundingBox& box)
  {
    root = (N3Node*)malloc(N*N*N*sizeof(N3Node));

    TriangleList tris; tris.reserve(triangles.size());
    for (unsigned i = 0; i < triangles.size(); i++)
      tris.push_back(&triangles[i]);

    while(1)
    {
      BOOST_FOREACH ( Triangle* tri, tris )
        tempNode.insert(tri);

      tempNode.normalize();

      for (unsigned z = 0; z < N; z++)
        for (unsigned y = 0; y < N; y++)
          for (unsigned x = 0; x < N; x++)
          if (!tempNode.triangles[x][y][z].empty())
          {
            newNode();
          }
    }


  }
*/

}
