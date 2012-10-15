(
 (nil . ())
 (c++-mode . (
           (elemc/cmake-debug-command . "mkdir -p ../build; pushd ../build; cmake -DCMAKE_BUILD_TYPE=Debug .. ; popd")
           (elemc/cmake-release-command . "mkdir -p ../build; pushd ../build; cmake -DCMAKE_BUILD_TYPE=Release .. ; popd")
           (elemc/compile-command . "pushd ../build; make -j2 -k; popd")
           )))
