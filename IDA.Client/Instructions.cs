using Idaas;

namespace Ida.Client
{
    public class Instructions
    {
        private readonly Idaas.Database.Client _client;

        public Instructions(Idaas.Database.Client client)
        {
            _client = client;
        }

        public IdaInstruction Fetch(int address)
        {
            return _client.fetchInstruction(address);
        }
    }
}