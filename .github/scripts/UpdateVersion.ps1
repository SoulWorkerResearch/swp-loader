$version = $Env:VERSION -replace '\.',', ' -replace '-',', '

$path =  '../../swp-sdk/headers/swpsdk/defines.hpp'
$content = Get-Content -Path $path

$content = $content -replace '(.*?current_version{) .*',('$1 ' + $version + ' };')

Set-Content -Path $path -Value $content