def main [type: string] {
  $">>> Running e4 - ($type) mode"
  cd $"build/output/linux/($type)"
  ./e4 &
  cd ../../../..
}
