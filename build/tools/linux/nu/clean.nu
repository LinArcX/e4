def main [type: string] {
  $">>> Cleaning 'build/output/linux/($type)' directory"
  rm -r $"build/output/linux/($type)"
}
