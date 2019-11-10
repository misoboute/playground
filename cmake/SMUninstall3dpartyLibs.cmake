# This module examines various SM_UNINSTALL_* cmake flags and if a certain
# flag SM_UNINSTALL_<lib> is set to true, the corresponding 3rdparty library
# <lib> is uninstalled during CMake generation. These flags are intended to be
# used on CMake invocation command line to uninstall one or more of the
# 3rdparty libraries so that, for instance, sm_find_or_get will reinstall
# a new version. Include this module after SMUtil but before everything
# else.

# set(SM_UNINSTALL_NLOHMANNJSON 1)
# set(SM_UNINSTALL_NURBSLIB 1)

if (NOT SM_3RDPARTY_INSTALL_DIR)
    # Doesn't make sense to uninstall libs from "nowhere!"
    return()
endif (NOT SM_3RDPARTY_INSTALL_DIR)

if(SM_UNINSTALL_NLOHMANNJSON)
    message("Uninstalling NLohmann...")
    file(REMOVE ${SM_3RDPARTY_INSTALL_DIR}/include/nlohmann/json.hpp)
endif(SM_UNINSTALL_NLOHMANNJSON)

if(SM_UNINSTALL_BOOST)
    message("Uninstalling Boost...")
    file(GLOB _boost_files
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libboost_*
    )
    file(REMOVE_RECURSE
        ${SM_3RDPARTY_INSTALL_DIR}/include/boost
        ${_boost_files}
    )
endif(SM_UNINSTALL_BOOST)

if(SM_UNINSTALL_FREECGI)
    message("Uninstalling freeCGI...")
    file(REMOVE_RECURSE
        ${SM_3RDPARTY_INSTALL_DIR}/include/freeCGI
        ${SM_3RDPARTY_INSTALL_DIR}/lib/freeCGI.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/freeCGId.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libfreeCGI.a
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libfreeCGId.a
    )
endif(SM_UNINSTALL_FREECGI)

if(SM_UNINSTALL_GMOCK)
    message("Uninstalling GTest and GMock...")
    file(GLOB _gmock_files
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libgmock*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libgtest*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/gmock*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/gtest*
    )
    file(REMOVE_RECURSE
        ${SM_3RDPARTY_INSTALL_DIR}/include/gmock
        ${SM_3RDPARTY_INSTALL_DIR}/include/gtest
        ${_gmock_files}
    )
endif(SM_UNINSTALL_GMOCK)

if(SM_UNINSTALL_GDCM)
    file(GLOB _gdcm_files
        ${SM_3RDPARTY_INSTALL_DIR}/include/gdcm-*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/gdcm-*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/gdcmopenjpeg-*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/gdcm*.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libgdcm*.a
        ${SM_3RDPARTY_INSTALL_DIR}/share/gdcm-*
    )
    message("Uninstalling GDCM... : \n${_gdcm_files}")
    file(REMOVE_RECURSE
        ${SM_3RDPARTY_INSTALL_DIR}/bin/concrt140.dll
        ${SM_3RDPARTY_INSTALL_DIR}/bin/msvcp140.dll
        ${SM_3RDPARTY_INSTALL_DIR}/bin/vcruntime140.dll
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libsocketxx.a
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libsocketxxd.a
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libvtkgdcm.a
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libvtkgdcmd.a
        ${SM_3RDPARTY_INSTALL_DIR}/lib/socketxx.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/socketxxd.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/vtkgdcm.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/vtkgdcmd.lib
        ${_gdcm_files}
    )
endif(SM_UNINSTALL_GDCM)

if(SM_UNINSTALL_ITK)
    file(GLOB _itk_files
        ${SM_3RDPARTY_INSTALL_DIR}/bin/itk*
        ${SM_3RDPARTY_INSTALL_DIR}/include/ITK-*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/cmake/ITK-*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libitk*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libITK*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/ITK*
        ${SM_3RDPARTY_INSTALL_DIR}/share/doc/ITK-*
    )
    message("Uninstalling ITK... : \n${_itk_files}")
    file(REMOVE_RECURSE
        ${SM_3RDPARTY_INSTALL_DIR}/cmake/hdf5
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libhdf5.settings
        ${_itk_files}
    )
endif(SM_UNINSTALL_ITK)

if(SM_UNINSTALL_ITK_4_13)
    file(GLOB _itk413_files
        ${SM_3RDPARTY_INSTALL_DIR}/include/ITK-4.13
        ${SM_3RDPARTY_INSTALL_DIR}/lib/cmake/ITK-4.13
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libitk*4.13*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libITK*4.13*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/ITK*4.13*
        ${SM_3RDPARTY_INSTALL_DIR}/share/doc/ITK-4.13
    )
    message("Uninstalling ITK 4.13 ... : \n${_itk413_files}")
    file(REMOVE_RECURSE ${_itk413_files})
endif(SM_UNINSTALL_ITK_4_13)

if(SM_UNINSTALL_NURBSLIB)
    message("Uninstalling nurbslib...")
    file(REMOVE_RECURSE
        ${SM_3RDPARTY_INSTALL_DIR}/include/nurbslib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/nurbslib.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/nurbslibd.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libnurbslib.a
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libnurbslibd.a
    )
endif(SM_UNINSTALL_NURBSLIB)

if(SM_UNINSTALL_QT_CIPHER_SQLITE_PLUGIN)
    message("Uninstalling QtCipherSqlitePlugin ...")
    file(REMOVE
        ${QTDIR}/plugins/sqldrivers/sqlitecipherd.dll
        ${QTDIR}/plugins/sqldrivers/sqlitecipherd.pdb
        ${QTDIR}/plugins/sqldrivers/sqlitecipher.dll
        ${QTDIR}/plugins/sqldrivers/sqlitecipher.dylib
    )
endif(SM_UNINSTALL_QT_CIPHER_SQLITE_PLUGIN)

if(SM_UNINSTALL_VTK)
    file(GLOB _vtk_files
        ${SM_3RDPARTY_INSTALL_DIR}/bin/vtk*
        ${SM_3RDPARTY_INSTALL_DIR}/include/vtk-*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/cmake/vtk-*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/vtk*.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libvtk*.a
        ${SM_3RDPARTY_INSTALL_DIR}/share/doc/vtk-*
    )
    list(REMOVE_ITEM _vtk_files
        ${SM_3RDPARTY_INSTALL_DIR}/lib/vtkgdcm.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/vtkgdcmd.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libvtkgdcm.a
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libvtkgdcmd.a
    )
    if(_vtk_files)
        message("Uninstalling VTK... : \n${_vtk_files}")
        file(REMOVE_RECURSE ${_vtk_files})
    endif(_vtk_files)
endif(SM_UNINSTALL_VTK)

if(SM_UNINSTALL_VTK_8_1)
    file(GLOB _vtk81_files
        ${SM_3RDPARTY_INSTALL_DIR}/bin/vtk*8.1*
        ${SM_3RDPARTY_INSTALL_DIR}/include/vtk-8.1
        ${SM_3RDPARTY_INSTALL_DIR}/lib/cmake/vtk-8.1
        ${SM_3RDPARTY_INSTALL_DIR}/lib/vtk*8.1.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libvtk*8.1.a
        ${SM_3RDPARTY_INSTALL_DIR}/share/doc/vtk-8.1
    )
    if(_vtk81_files)
        message("Uninstalling VTK 8... : \n${_vtk81_files}")
        file(REMOVE_RECURSE ${_vtk81_files})
    endif(_vtk81_files)
endif(SM_UNINSTALL_VTK_8_1)

if(SM_UNINSTALL_WXWIDGETS)
    message("Uninstalling wxWidgets...")
    file(GLOB _wx_files
        ${SM_3RDPARTY_INSTALL_DIR}/bin/wx*
        ${SM_3RDPARTY_INSTALL_DIR}/include/wx*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/vc*/wx*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/vc*/mswu*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libwx*
        ${SM_3RDPARTY_INSTALL_DIR}/lib/wx
    )
    file(REMOVE_RECURSE ${_wx_files})
    if(_wx_files)
        message("Uninstalling VTK 8... : \n${_wx_files}")
        file(REMOVE_RECURSE ${_wx_files})
    endif(_wx_files)
endif(SM_UNINSTALL_WXWIDGETS)

if(SM_UNINSTALL_ZLIB)
    message("Uninstalling Zlib...")
    file(GLOB _zlib_unix_files
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libz.*.dylib
    )
    file(REMOVE
        ${SM_3RDPARTY_INSTALL_DIR}/include/zlib.h
        ${SM_3RDPARTY_INSTALL_DIR}/include/zconf.h
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libz.a
        ${SM_3RDPARTY_INSTALL_DIR}/lib/libzd.a
        ${SM_3RDPARTY_INSTALL_DIR}/lib/zlib.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/zlibstatic.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/zlibd.lib
        ${SM_3RDPARTY_INSTALL_DIR}/lib/zlibstaticd.lib
        ${SM_3RDPARTY_INSTALL_DIR}/share/man/man3/zlib.3
        ${SM_3RDPARTY_INSTALL_DIR}/share/pkgconfig/zlib.pc
        ${_zlib_unix_files}
    )
endif(SM_UNINSTALL_ZLIB)
