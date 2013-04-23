using Idaas;
using NUnit.Framework;

namespace Ida.Client.Test
{
    [TestFixture]
    public class DescribeInstructions : DatabaseSpec
    {
        [Test]
        public void ItShouldParseRegisterOperand()
        {
            var initInstance = Database.Functions.FindByName("InitInstance");
            var instruction = Database.Instructions.Fetch(initInstance.StartAddress);
            Assert.That(instruction.Mnemonic, Is.EqualTo("push"));
            Assert.That(instruction.Operands.Count, Is.EqualTo(1));
            var operand = instruction.Operands[0];
            Assert.That(operand.Type, Is.EqualTo(IdaOperandType.Register));
            Assert.That(operand.Size, Is.EqualTo(4));
            Assert.That(operand.BaseRegister, Is.EqualTo(IdaRegister.Ebp));
        }
    }
}