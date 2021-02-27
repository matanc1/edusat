$studentid="matan-lr-rsr"

$DestinationBasePath="c:\trial\results"
$Destination="$($DestinationBasePath)\$($studentid)"
$ResourceGroupName="la096265course-infrastructure-group"
$StorageName="la096265datastorage"
$storageAccount = Get-azStorageAccount -ResourceGroupName "$($ResourceGroupName)" -Name "$($StorageName)"

mkdir $Destination

$outputs = Get-AzStorageBlob -Container tasks -Context $storageAccount.Context -Prefix "$($studentid)/output" -MaxCount 1000000
foreach($output in $outputs)
{
    if ($output.Name.EndsWith("statistics.txt"))
    {
        write-host $output.Name
        $resultname= $output.Name.Split("/")[-3]
        if (!(test-path "$($Destination)\$($resultname)")) {
            write-Host $($output.Name)
            Get-AzStorageBlobContent -Container tasks -Context $storageAccount.Context -Blob "$($output.Name)" -Destination "$($Destination)\$($resultname).txt"
        }
    }    
}
