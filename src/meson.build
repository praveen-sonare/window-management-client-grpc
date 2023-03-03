pkgconfig = import('pkgconfig')
cpp = meson.get_compiler('cpp')

qt5_dep = dependency('qt5', modules: ['Qml', 'Quick', 'Gui'])
dep_qtappfw = [
    dependency('qtappfw-weather'),
]

grpcpp_reflection_dep = cpp.find_library('grpc++_reflection')
protoc = find_program('protoc')
grpc_cpp = find_program('grpc_cpp_plugin')

protoc_gen = generator(protoc, \
                       output : ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
                       arguments : ['--proto_path=@CURRENT_SOURCE_DIR@/',
                         '--cpp_out=@BUILD_DIR@',
                         '@INPUT@'])

generated_protoc_sources = protoc_gen.process('agl_shell.proto')

grpc_gen = generator(protoc, \
                     output : ['@BASENAME@.grpc.pb.cc', '@BASENAME@.grpc.pb.h'],
                     arguments : ['--proto_path=@CURRENT_SOURCE_DIR@/',
                       '--grpc_out=@BUILD_DIR@',
                       '--plugin=protoc-gen-grpc=' + grpc_cpp.path(),
                       '@INPUT@'])
generated_grpc_sources = grpc_gen.process('agl_shell.proto')

grpc_deps = [
    dependency('protobuf'),
    dependency('grpc'),
    dependency('grpc++'),
    grpcpp_reflection_dep,
]

window_mgnt_dep = [
    qt5_dep,
    dep_qtappfw,
    grpc_deps,
]

window_mgnt_resources = [
  'qml/images/images.qrc',
  'qml/qml.qrc'
]

resource_files = qt5.compile_resources(sources: window_mgnt_resources)

window_mgnt_src_headers = [
  'AglShellGrpcClient.h',
]

moc_files = qt5.compile_moc(headers: window_mgnt_src_headers,
                            dependencies: qt5_dep)
window_mgnt_src = [
  'AglShellGrpcClient.cpp',
  'main.cpp',
  generated_protoc_sources,
  generated_grpc_sources,
]

prefix_path = get_option('prefix')
binplugin_dir = join_paths(prefix_path, get_option('bindir'))

executable('window-management-rpc', window_mgnt_src, resource_files, moc_files,
            dependencies : window_mgnt_dep,
            install_rpath: binplugin_dir,
            install: true)
