& cmake --build ./build 

if ($? -eq $true)
{
  & ./build/pong.exe

} else {
  Write-Host "build failed"
}
