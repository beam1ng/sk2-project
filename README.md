To build server:
1. Navigate to sk2-project/server.
2. Create folder "build".
3. Run "cmake .."
4. Run "make"
5. At this point, an executable named "server" should be present in the build folder.
6. To start the server on port X Run "./server X"

To build client:
1. Navigate to sk2-project/client.
2. Create folder "build".
3. Run "/opt/Qt/6.4.2/gcc_64/bin/qmake ../client.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug",
where /opt/Qt/6.4.2/gcc_64/bin/qmake is an example location of your qt qmake location.
4. Run "make"
5. At this point, an executable named "client" should be present in the build folder.
6. To start the client Run "./clientX"
