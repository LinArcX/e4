def main [type: string] {
  $">>> Running e4 - ($type) mode"
  cd $"build/output/windows/($type)"
  ./e4 &
  cd ../../../..
}
