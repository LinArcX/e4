def main [type: string] {
  $">>> Cleaning 'build/output/windows/($type)' directory"
  rm -r $"build/output/windows/($type)"
}
