{
  'targets': [
    {
      'target_name': 'pontuModuleNative',
      'sources': [
        'src/module/addon.c',
        'src/module/cloud_wrappers.c',
        'src/module/common.c',
        'src/module/converters.c',
        'src/module/registration_wrappers.c',
        'src/algebra.c',
        'src/calc.c',
        'src/chebyshev.c',
        'src/cloud.c',
        'src/dataframe.c',
        'src/extraction.c',
        'src/harmonics.c',
        'src/hu.c',
        'src/kdtree.c',
        'src/legendre.c',
        'src/matrix.c',
        'src/octree.c',
        'src/plane.c',
        'src/pointset.c',
        'src/registration.c',
        'src/spheric.c',
        'src/vector3.c',
        'src/voxelgrid.c',
        'src/zernike.c'
        ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      }
    }
  ]
}