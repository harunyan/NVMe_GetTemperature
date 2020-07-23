using System;

namespace NVMe_GetTemperature
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                Console.WriteLine("Usage:");
                Console.WriteLine("GetTemperature.exe [CPU|HDD]");

                return;
            }

            var arg = args[0];

            //var measurement = "cpu_storage_temperature";
            //var datetime = DateTime.Now.ToUniversalTime();

            //var cpus = Cpu.Discover();
            //var hdds = HardDrive.Discover();

            //int corecount, cpucount = 0;
            switch (arg)
            {
                //case "CPU":
                //    //var timestamp = (long)datetime.Subtract(datetimeUnixEpoch).TotalSeconds;
                //    corecount = 0;
                //    foreach (var cpu in cpus)
                //    {
                //        foreach (var core in cpu.CoreTemperatures)
                //        {
                //            //Console.WriteLine($"temp.core{corecount}\t{core.Value}\t{timestamp}");
                //            Console.WriteLine($"{datetime}\ttemp.core{corecount}\t{core.Value}");
                //            corecount++;
                //        }
                //    }

                //    break;

                case "HDD":
                    foreach (var hdd in hdds)
                    {
                        Console.WriteLine($"{hdd.Name}");
                        foreach (var smart in hdd.SmartAttributes)
                        {
                            if (smart.Identifier == 194)
                            {
                                Console.WriteLine($"{datetime}\t{smart.ParameterDescriptions[0].Value}");
                            }
                        }
                    }

                    break;

                default:
                    break;
            }
        }
    }
}
