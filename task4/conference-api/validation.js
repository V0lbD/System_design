const database = db.getSiblingDB("conference_mongo");

print("\n=== MongoDB schema validation for users collection ===");

const validationResult = database.runCommand({
  collMod: "users",
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: [
        "login",
        "password_hash",
        "first_name",
        "last_name",
        "role",
        "email",
        "created_at",
        "updated_at"
      ],
      properties: {
        _id: {
          bsonType: "objectId",
          description: "_id must be an ObjectId"
        },
        login: {
          bsonType: "string",
          pattern: "^[a-zA-Z0-9_]{3,30}$",
          description: "login is required and must contain 3-30 latin letters, digits or underscores"
        },
        password_hash: {
          bsonType: "string",
          minLength: 6,
          description: "password_hash is required and must be a string with at least 6 characters"
        },
        first_name: {
          bsonType: "string",
          minLength: 1,
          description: "first_name is required and must be a non-empty string"
        },
        last_name: {
          bsonType: "string",
          minLength: 1,
          description: "last_name is required and must be a non-empty string"
        },
        role: {
          enum: ["participant", "speaker", "organizer"],
          description: "role must be one of participant, speaker, organizer"
        },
        email: {
          bsonType: "string",
          pattern: "^[^@\\s]+@[^@\\s]+\\.[^@\\s]+$",
          description: "email is required and must have email-like format"
        },
        bio: {
          bsonType: "string",
          description: "bio must be a string"
        },
        contacts: {
          bsonType: "object",
          properties: {
            telegram: {
              bsonType: "string",
              description: "telegram must be a string"
            },
            github: {
              bsonType: "string",
              description: "github must be a string"
            }
          }
        },
        interests: {
          bsonType: "array",
          items: {
            bsonType: "string"
          },
          description: "interests must be an array of strings"
        },
        rating: {
          bsonType: ["double", "int", "long", "decimal"],
          minimum: 0,
          maximum: 5,
          description: "rating must be a number from 0 to 5"
        },
        created_at: {
          bsonType: "date",
          description: "created_at is required and must be a date"
        },
        updated_at: {
          bsonType: "date",
          description: "updated_at is required and must be a date"
        }
      }
    }
  },
  validationLevel: "strict",
  validationAction: "error"
});

print("Validation command result:");
printjson(validationResult);

const validUserId = ObjectId("660000000000000000000098");
const invalidUserId = ObjectId("660000000000000000000097");

database.users.deleteOne({ _id: validUserId });
database.users.deleteOne({ _id: invalidUserId });

print("\n=== Try to insert valid user ===");

try {
  database.users.insertOne({
    _id: validUserId,
    login: "valid_user",
    password_hash: "hash_valid_user",
    first_name: "Valid",
    last_name: "User",
    role: "participant",
    email: "valid_user@example.com",
    bio: "User inserted to test schema validation",
    contacts: {
      telegram: "@valid_user",
      github: "valid-user"
    },
    interests: ["mongodb", "validation"],
    rating: 4.0,
    created_at: new Date(),
    updated_at: new Date()
  });

  print("Valid user inserted successfully.");
  printjson(database.users.findOne({ _id: validUserId }));
} catch (error) {
  print("ERROR: valid user was rejected.");
  print(error.message);
}

print("\n=== Try to insert invalid user ===");

try {
  database.users.insertOne({
    _id: invalidUserId,

    // login is invalid: too short and contains invalid character
    login: "x!",

    // password_hash is invalid: too short
    password_hash: "123",

    first_name: "",
    last_name: "Invalid",

    // role is invalid: not in enum
    role: "admin",

    // email is invalid
    email: "not-an-email",

    // rating is invalid: greater than maximum 5
    rating: 10,

    // created_at is invalid: string instead of Date
    created_at: "2026-05-20",

    updated_at: new Date()
  });

  print("ERROR: invalid user was inserted, validation did not work.");
} catch (error) {
  print("Invalid user rejected as expected.");
  print("Error message:");
  print(error.message);
}

print("\n=== Try invalid update ===");

try {
  database.users.updateOne(
    { _id: validUserId },
    {
      $set: {
        rating: 7,
        updated_at: new Date()
      }
    }
  );

  print("ERROR: invalid update was accepted.");
} catch (error) {
  print("Invalid update rejected as expected.");
  print("Error message:");
  print(error.message);
}

print("\n=== Cleanup ===");

database.users.deleteOne({ _id: validUserId });
database.users.deleteOne({ _id: invalidUserId });

print("Valid test users after cleanup: " + database.users.countDocuments({ _id: validUserId }));
print("Invalid test users after cleanup: " + database.users.countDocuments({ _id: invalidUserId }));

print("\nSchema validation script completed successfully.");
