$version = $Env:VERSION -replace '\.',', '

$path =  '../../swp-sdk/defines.hpp'
$content = Get-Content -Path $path

$content = $content -replace '(.*?current_version{) .*',('$1 ' + $version + ' };')

Set-Content -Path $path -Value $content