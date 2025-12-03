#pragma once
enum class MessageIntegrity {
	UnableToCheck,// No CRC provided — cannot check
    Unverified,       // Subsystem did not attempt to verify
    VerifiedPassed,   // CRC present + matched
    VerifiedFailed    // CRC present + did NOT match
};