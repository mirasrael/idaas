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
            Assert.That(operand.Register_, Is.EqualTo(IdaRegister.Ebp));            
        }

        [Test]
        public void ItShouldParseConstantOperand()
        {
            var instruction = Database.Instructions.Fetch(0x00411583);
            Assert.That(instruction.Mnemonic, Is.EqualTo("sub"));
            Assert.That(instruction.Operands.Count, Is.EqualTo(2));
            var operand = instruction.Operands[1];
            Assert.That(operand.Type, Is.EqualTo(IdaOperandType.Constant));
            Assert.That(operand.Size, Is.EqualTo(4));
            Assert.That(operand.Value, Is.EqualTo(0xCC));
        }

        [Test]
        public void ItShouldParseMemoryOperand()
        {
            var instruction = Database.Instructions.Fetch(0x004117CE);
            Assert.That(instruction.Mnemonic, Is.EqualTo("mov"));
            Assert.That(instruction.Operands.Count, Is.EqualTo(2));
            var operand = instruction.Operands[1];
            Assert.That(operand.Type, Is.EqualTo(IdaOperandType.Memory));
            Assert.That(operand.Size, Is.EqualTo(4));
            Assert.That(operand.IndexRegister, Is.EqualTo(IdaRegister.None));
            Assert.That(operand.Address, Is.EqualTo(0x00419000));
        }

        [Test]
        public void ItShouldParseDisplacementOperand()
        {
            var instruction = Database.Instructions.Fetch(0x0041159E);
            Assert.That(instruction.Mnemonic, Is.EqualTo("mov"));
            Assert.That(instruction.Operands.Count, Is.EqualTo(2));
            var operand = instruction.Operands[1];
            Assert.That(operand.Type, Is.EqualTo(IdaOperandType.Displacement));
            Assert.That(operand.Size, Is.EqualTo(4));
            Assert.That(operand.BaseRegister, Is.EqualTo(IdaRegister.Ebp));
            Assert.That(operand.IndexRegister, Is.EqualTo(IdaRegister.None));
            Assert.That(operand.Address, Is.EqualTo(0x08));
        }

        [Test]
        public void ItShouldParseAddressOperand()
        {
            var instruction = Database.Instructions.Fetch(0x00411807);
            Assert.That(instruction.Mnemonic, Is.EqualTo("jmp"));
            Assert.That(instruction.Operands.Count, Is.EqualTo(1));
            var operand = instruction.Operands[0];
            Assert.That(operand.Type, Is.EqualTo(IdaOperandType.Address));
            Assert.That(operand.Size, Is.EqualTo(4));
            Assert.That(operand.Address, Is.EqualTo(0x4118EB));
        }
    }
}